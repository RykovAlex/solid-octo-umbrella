#pragma once
#include "optimapoint.h"
#include "optimacorner.h"
#include "optimaline.h"
#include "OptimaCross.h"

class OptimaPath
{
public:
	OptimaPath();

	OptimaPath(const QPainterPath &points);

	OptimaPath(const QPointF &startPoint, const OptimaCross & cross);

	OptimaPath(const QPointF &startPoint);

	~OptimaPath()
	{
	}

	int linesCount() const;
	
	const OptimaLine & lineAt(int i) const;
	
	int cornersCount() const;
	
	const OptimaCorner& cornerAt(int i) const;
	
	void lineTo(const QPointF &endPoint);
	
	void lineTo(const QPointF &endPoint, QLineF nextLine, qreal radius);
	
	const QPainterPath toPath() const;

	QVector<qreal> intersected(const OptimaLine & otherLine, int start = 0);

	void intersected(OptimaPath & path);

	void clearIntersection();

protected:


private:
	OptimaCross mCross;

	QVector<OptimaLine> mLines;
	
	QPointF mCurrentPosition;

	void initialize(const QPainterPath & points);

	template <class T>
	bool getIntersectionPoint(const T & line, const OptimaLine &otherLine, QPointF *intersectionPoint) const;

	qreal getLineSublength(const OptimaLine &line, const QPointF & point) const;

};

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug, const OptimaPath &);
#endif


