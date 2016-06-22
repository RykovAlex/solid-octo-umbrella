#include "stdafx.h"
#include "tag.h"
#include "optimapoint.h"

OptimaPoint::OptimaPoint() 
	: mUuid(QUuid::createUuid( ).toString( ).toLower( ).remove( "{" ).remove( "}" ))
{
}

OptimaPoint::OptimaPoint(QDomNode nodeDot)
{
	apply(nodeDot);
}

OptimaPoint::OptimaPoint(qreal xpos, qreal ypos, int radius) 
	: QPointF(xpos, ypos)
	, mRadius(radius)
	, mUuid(QUuid::createUuid( ).toString( ).toLower( ).remove( "{" ).remove( "}" ))
{
}

OptimaPoint::OptimaPoint(const QPointF &point, int radius) 
	: QPointF(point)
	, mRadius(radius)
	, mUuid(QUuid::createUuid( ).toString( ).toLower( ).remove( "{" ).remove( "}" ))
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

void OptimaPoint::apply(const QDomNode & nodeDot)
{
	QDomNode nodeCoordinate = nodeDot.namedItem(tag::coordinate);
	
	//ѕроверим в каком формате передана структура точки
	if (nodeCoordinate.isNull())
	{
		//ѕросто точка в формате x:y[:radius] дл€ всего остального
		initialize(nodeDot.toElement( ).text( ));
	}
	else
	{
		//Ёто структура коннектора
		initialize(nodeCoordinate.toElement( ).text( ));
		QDomNode nodeId = nodeDot.namedItem(tag::id);
		if (!nodeId.isNull())
		{
			setUuid(nodeId.toElement( ).text( ));
		}
	}
}

