#include "stdafx.h"
#include "tag.h"
#include "optimafigure.h"

OptimaFigure::OptimaFigure(const QString &uuid) : OptimaElement(uuid)
{
	QPainterPath path( QPointF(0,0));
	path.lineTo(0,100);
	path.lineTo(100,100);
	path.lineTo(100,0);
	path.lineTo(0,0);
	setPath( path );
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
	
	setData(tag::data::id, uuid);
}
