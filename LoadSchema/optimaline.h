#pragma once
#include "QtCore\QLine"
#include "optimacorner.h"

class OptimaLine : public QLineF
{
public:
	OptimaLine(const QPointF &pt1, const QPointF &pt2);
	
	OptimaLine();

	~OptimaLine()
	{
	}

	void setCorner(const QPointF &beginPoint, const QPointF &cornerPoint, const QPointF &endPoint);
	
	void setCorner(const OptimaCorner &corner);

	const OptimaCorner &corner() const;

	void toPath(QPainterPath &path) const;
protected:

private:
	OptimaCorner mOptimaCorner;

};


