#pragma once
#include "QPolygon"
#include "QLine"
#include "QPoint"

class OptimaCorner : public QPolygonF
{
public:
	OptimaCorner();

	OptimaCorner(const QPointF &startPoint, const QPointF &cornerPoint, const QPointF &endPoint );

	~OptimaCorner()
	{
	}

	///передвинуть eугол на нлвле место
	void translate(const QPointF & offset);

	QLineF::IntersectType  intersect ( const QLineF & line, QPointF * intersectionPoint ) const;

protected:

private:
	QLineF line1;
	
	QLineF line2;
	
	qreal length(const QPointF & p1, const QPointF & p2) const;

	QLineF::IntersectType intersectApproximately( const QLineF &line, QPointF *intersectionPoint, const qreal t0, const qreal t1 ) const;
};

