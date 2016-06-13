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
	// �������� ������ �������
	const qreal breakLineLength = 5.;

	QLineF currentLine( originalLine );

	// ��� ����������� ����������� ����������� ����� ������ ���� ������������� �� �����������
	qSort(crossingLengths);
	
	// ����� ���� ��������� ��������� ����� ����� �����������
	qreal lastLength = 0;
	for ( int i = 0; i < crossingLengths.count(); i++ )
	{
		// ���� ����������� ��������� �� ����� ���������� �� ����������� ��� ����� � ���������� �����������,
		// ����� ������ ������
		if ( lastLength >= crossingLengths.at(i)) {
			continue;
		}

		lastLength = crossingLengths.at(i);

		// ������ ����� �������
		QPointF breakingPoint_p1;

		// ���� ����� ������� ������ ���� ����� ���������� ����� ����� �� ������ ����� ����������
		if ( lastLength <= breakLineLength)
		{
			breakingPoint_p1 = currentLine.p1();
		} 
		else
		{
			// lengthFromBegin ��������� ������� ��������� �������
			currentLine.setLength( lastLength - breakLineLength);
			breakingPoint_p1 = currentLine.p2();
		}

		// ������ ����� �������
		path.lineTo( breakingPoint_p1 );

		// �������������� ����� �������
		qreal endCrossingLength = lastLength + breakLineLength;

		// �������� ��� ����� ���������� ��������� ����� ������� � ����
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

		// ������ ����� �������		
		QPointF breakingPoint_p2( currentLine.p2() );

		// ���� ������ �� ������� �� ������� �������
		if ( currentLine.length() <= originalLine.length() ) 
		{
			switch ( mCrossType ) 
			{
				case connector_cross_bridge:
					{
						// ������� �������
						QLineF breakLine( breakingPoint_p1, breakingPoint_p2 );
						// ����������� �������
						QRectF breakEllipseQuad( breakLine.pointAt( 0.5 ) + QPointF( breakLine.length() / 2, breakLine.length() / 2 ), breakLine.pointAt( 0.5 ) - QPointF( breakLine.length() / 2, breakLine.length() / 2 ) );
						// ����������� ���� bridge
						path.arcTo ( breakEllipseQuad, originalLine.angle(), 180.0  * (originalLine.angle() > 180 ? -1 : 1 ));				
					}
					break;
				case connector_cross_break:
					// ��������� ������ ������������
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
