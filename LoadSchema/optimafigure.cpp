#include "stdafx.h"
#include "tag.h"
#include "optimafigure.h"

OptimaFigure::OptimaFigure(const QString &itemUuid) : OptimaElement(this)
{
	setData(tag::data::uuid, itemUuid);

	QPainterPath path( QPointF(0,0));
	path.lineTo(0,100);
	path.lineTo(100,100);
	path.lineTo(100,0);
	path.lineTo(0,0);
	setPath( path );
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);

}

void OptimaFigure::apply(const QDomNode & figure)
{
	applyXml(figure);

	const QDomNodeList dn_dots = figure.namedItem( tag::structure_dot ).toElement( ).childNodes( );
	for ( int mm = 0; mm < dn_dots.size( ); ++mm )
	{
		fd.m_points << convert( dn_dots.at( mm ).toElement( ).text( ) );
	}
	fd.m_points << fd.m_points.first( );

}