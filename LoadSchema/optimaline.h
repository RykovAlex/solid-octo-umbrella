#pragma once
#include "QtCore\QLine"
#include "optimacorner.h"
#include "OptimaCross.h"
#include "optimapoint.h"

class OptimaCross;

class OptimaLine : public QLineF
{
public:
	OptimaLine();

	OptimaLine(const QPointF &pt1, const QPointF &pt2);
	
	OptimaLine(const QLineF line, const QVector<QString> uuids);

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

	QString uuid1() const
	{
		return mUuids.at(0);
	}

	QString uuid2() const
	{
		return mUuids.at(1);
	}

	///передвинуть eугол на нлвле место
	void translate(const QPointF & offset);

protected:

private:
	QVector<QString> mUuids; ///<Иды точек концов отрезка
	
	OptimaCorner mCorner; ///<Угол до следующей линии

	OptimaLengthVector mCrossingWithConnectorLengths;///<Массив расстояний от первой точки отрезка, до точек пересечения с другими коннекторами
};

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug, const OptimaLine &);
#endif

