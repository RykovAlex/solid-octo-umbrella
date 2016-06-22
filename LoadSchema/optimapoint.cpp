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
		throw QObject::tr( "������ ������� ����������, ������ ����������" );
	}

	const QStringList pointsList = text.split( ":" );
	if ( pointsList.size( ) < 2 )
	{
		throw QObject::tr( "������ ������� ����������, ��������� ����������" );
	}

	// ���� 2 ��������, �� ��� ������ ����, ���� 3 �� ������ �������� ��� ������
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
	
	//�������� � ����� ������� �������� ��������� �����
	if (nodeCoordinate.isNull())
	{
		//������ ����� � ������� x:y[:radius] ��� ����� ����������
		initialize(nodeDot.toElement( ).text( ));
	}
	else
	{
		//��� ��������� ����������
		initialize(nodeCoordinate.toElement( ).text( ));
		QDomNode nodeId = nodeDot.namedItem(tag::id);
		if (!nodeId.isNull())
		{
			setUuid(nodeId.toElement( ).text( ));
		}
	}
}

