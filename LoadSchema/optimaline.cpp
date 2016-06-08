#include "stdafx.h"
#include "optimaline.h"

OptimaLine::OptimaLine(const QPointF &pt1, const QPointF &pt2) :
QLineF(pt1, pt2)
{

}

OptimaLine::OptimaLine()
{

}

void OptimaLine::setCorner(const QPointF &beginPoint, const QPointF &cornerPoint, const QPointF &endPoint)
{
	mOptimaCorner.clear();
	mOptimaCorner << beginPoint << cornerPoint << endPoint;
}

void OptimaLine::setCorner(const OptimaCorner &corner)
{
	mOptimaCorner = corner;
}

const OptimaCorner & OptimaLine::corner() const
{
	return mOptimaCorner; 
}

void OptimaLine::toPath(QPainterPath &path) const
{
	if (path.currentPosition() != p1())
	{
		path.moveTo(p1());
	}	
	path.lineTo(p2());
	if (mOptimaCorner.count() == 3)
	{
		path.quadTo(mOptimaCorner.at(1), mOptimaCorner.at(2));
	}
}
