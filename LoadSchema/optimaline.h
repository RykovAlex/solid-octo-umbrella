#pragma once
#include "QtCore\QLine"
#include "optimacorner.h"
#include "OptimaCross.h"

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

	void toPath(QPainterPath &path, const OptimaCross & cross) const;

	void addIntersectedLength(const QVector<qreal> & intersectedLengths);

	void addIntersectedLength(qreal intersectedLength);

	void clearIntersectedLength();

	bool contains(const QPointF & intersectionPoint) const;

	QVector<qreal> getCrossingLength() const;

	void sortLengths();


protected:

private:
	OptimaCorner mOptimaCorner; ///<Угол до следующей линии

	QVector<qreal> mCrossingWithConnectorLengths;///<Массив расстояний от первой точки отрезка, до точек пересечения с другими коннекторами

};


