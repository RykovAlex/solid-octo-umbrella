#include "stdafx.h"
#include "optimacorner.h"

OptimaCorner::OptimaCorner(const QPointF &startPoint, const QPointF &cornerPoint, const QPointF &endPoint) 
	: line1(startPoint, cornerPoint)
	, line2(cornerPoint, endPoint)

{
	push_back(startPoint);
	push_back(cornerPoint);
	push_back(endPoint);
}

OptimaCorner::OptimaCorner()
{

}

void OptimaCorner::translate(const QPointF & offset)
{
	for (int i = 0; i < size(); ++i)
	{
		(*this)[i] += offset;
	}

	line1.translate(offset);
	line2.translate(offset);
}

QLineF::IntersectType OptimaCorner::intersect(const QLineF & line, QPointF * intersectionPoint) const
{
	return intersectApproximately( line, intersectionPoint, 0., 1. );
}

qreal OptimaCorner::length(const QPointF & p1, const QPointF & p2) const
{
	QLineF l( p1, p2 );
	return l.length();
}

QLineF::IntersectType OptimaCorner::intersectApproximately(const QLineF &line, QPointF *intersectionPoint, const qreal t0, const qreal t1) const
{
	if (isEmpty())
	{
		return QLineF::NoIntersection;
	}

	qreal t = t0 + (t1 - t0) / 2;	
	
	QPointF p(QLineF(line1.pointAt(t), line2.pointAt(t)).pointAt(t));
	
	QLineF l1( line1.p1(), p );
	if ( l1.intersect( line, intersectionPoint ) == QLineF::BoundedIntersection )
	{
		if ( length( p, *intersectionPoint ) <= 1.0 )
		{
			return QLineF::BoundedIntersection;
		}
		return intersectApproximately( line, intersectionPoint, t0, t );
	}

	QLineF l2( p, line2.p2() );
	if ( l2.intersect( line, intersectionPoint ) == QLineF::BoundedIntersection )
	{
		if ( length( p, *intersectionPoint ) <= 1.0 )
		{
			return QLineF::BoundedIntersection;
		}
		return intersectApproximately( line, intersectionPoint, t, t1 );
	}

	return QLineF::NoIntersection;
}
