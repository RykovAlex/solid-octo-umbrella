#pragma once
#include "QtGui\QPolygon"

class OptimaCorner : public QPolygonF
{
public:
	OptimaCorner(const QPointF &startPoint, const QPointF &cornerPoint, const QPointF &endPoint )
	{
		push_back(startPoint);
		push_back(cornerPoint);
		push_back(endPoint);
	}

	~OptimaCorner()
	{
	}

protected:

private:

};


