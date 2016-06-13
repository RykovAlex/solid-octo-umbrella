#include "stdafx.h"
#include "optimacross.h"

OptimaCross::OptimaCross() : mCrossType(connector_cross_full)
{

}

void OptimaCross::apply(const QString &crossType)
{
	if ( crossType == "CrossBreak" )
		mCrossType = connector_cross_break;
	else 
	if ( crossType == "CrossBridge" )
		mCrossType = connector_cross_bridge;
	else
		mCrossType = connector_cross_full;
}

void OptimaCross::draw(QPainterPath &path, const QLineF & originalLine, OptimaPoint::OptimaLengthVector crossingLengths) const
{
	// половина ширины разрыва
	const qreal breakLineLength = 5.;

	QLineF currentLine( originalLine );

	// для корректного отображения пересечений длина должны быть отсортированы по возрастанию
	qSort(crossingLengths);
	
	// здесь надо разорвать рисование около точки пересечения
	qreal lastLength = 0;
	for ( int i = 0; i < crossingLengths.count(); i++ )
	{
		// если пересечение произошло на точке коннектора то порождаются две точки с одинаковым расстоянием,
		// берем только первую
		if ( lastLength >= crossingLengths.at(i)) {
			continue;
		}

		lastLength = crossingLengths.at(i);

		// первая точка разрыва
		QPointF breakingPoint_p1;

		// если точка разрыва меньше либо равна промежутку тогда берем за начало точку коннектора
		if ( lastLength <= breakLineLength)
		{
			breakingPoint_p1 = currentLine.p1();
		} 
		else
		{
			// lengthFromBegin учитывает наличие начальной стрелки
			currentLine.setLength( lastLength - breakLineLength);
			breakingPoint_p1 = currentLine.p2();
		}

		// первая точка разрыва
		path.lineTo( breakingPoint_p1 );

		// предполагаемая длина разрыва
		qreal endCrossingLength = lastLength + breakLineLength;

		// проверим что нужно объединить несколько точек разрыва в одну
		for ( ; ( i + 1 ) < crossingLengths.count(); i++ )
		{
			if ( crossingLengths.at(i + 1) > endCrossingLength ) 
			{
				break;
			}
			
			qreal nextEndCrossingLength = crossingLengths.at(i + 1) + breakLineLength;
			if ( nextEndCrossingLength < originalLine.length() )
			{
				endCrossingLength = nextEndCrossingLength;
			}			
		}

		currentLine.setLength( endCrossingLength );

		// вторая точка разрыва		
		QPointF breakingPoint_p2( currentLine.p2() );

		// если разрыв не выходит за текущий отрезок
		if ( currentLine.length() <= originalLine.length() ) 
		{
			switch ( mCrossType ) 
			{
				case connector_cross_bridge:
					{
						// отрезок разрыва
						QLineF breakLine( breakingPoint_p1, breakingPoint_p2 );
						// описывающий квадрат
						QRectF breakEllipseQuad( breakLine.pointAt( 0.5 ) + QPointF( breakLine.length() / 2, breakLine.length() / 2 ), breakLine.pointAt( 0.5 ) - QPointF( breakLine.length() / 2, breakLine.length() / 2 ) );
						// пересечение типа bridge
						path.arcTo ( breakEllipseQuad, originalLine.angle(), 180.0  * (originalLine.angle() > 180 ? -1 : 1 ));				
					}
					break;
				case connector_cross_break:
					// оставляем пустое пространство
					path.moveTo ( breakingPoint_p2 );				
					break;
				case connector_cross_full:
					break;
				default:
					break;
			}
		}
	}
}
