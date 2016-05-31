#include "stdafx.h"
#include "tag.h"
#include "optimapoint.h"

OptimaPoint::OptimaPoint()
{

}

OptimaPoint::OptimaPoint(qreal xpos, qreal ypos, int radius) : QPointF( xpos, ypos), mRadius(radius)
{

}

int OptimaPoint::getRadius() const
{
	return mRadius;
}

void OptimaPoint::initialize( const QString & text )
{
	if ( text.isEmpty( ) )
	{
		throw QObject::tr( "ќшибка анализа координаты, пуста€ координата" );
	}

	const QStringList pointsList = text.split( ":" );
	if ( pointsList.size( ) < 2 )
	{
		throw QObject::tr( "ќшибка анализа координаты, недостает параметров" );
	}

	// если 2 элемента, то это просто угол, если 3 то третий параметр это радиус
	mRadius = 0;
	if ( pointsList.size( ) >= 3 )
	{
		mRadius = pointsList.at( 2 ).toInt( );
	}
	setX(pointsList.at( 0 ).toDouble( ));
	setY(pointsList.at( 1 ).toDouble( ));
}

OptimaPoint::OptimaPoint(QDomNode nodeDot)
{
	//ѕроверим в каком формате передана структура точки
	//if (nodeDot.hasChildNodes())
	//{
	//	//Ёто структура коннектора
	//	initialize(nodeDot.namedItem(tag::coordinate).toElement( ).text( ));
	//}
	//else
	//{
	//	//ѕросто точка в формате x:y[:radius] дл€ всего остального
	//	initialize(nodeDot.toElement( ).text( ));
	//}
	//ѕросто точка в формате x:y[:radius] дл€ всего остального
	initialize(nodeDot.toElement( ).text( ));

}

