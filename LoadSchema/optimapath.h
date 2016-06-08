#pragma once
#include "optimapoint.h"
#include "optimacorner.h"

class OptimaPath
{
public:
	OptimaPath(const QPainterPath &points)
	{
		initialize(points);
	}

	OptimaPath(const QPointF &startPoint): mCurrentPosition(startPoint)
	{
	}

	~OptimaPath()
	{
	}

	int linesCount() const;
	const QLineF& lineAt(int i) const;
	int cornersCount() const;
	const OptimaCorner& cornerAt(int i) const;
	void lineTo(const QPointF &endPoint);
	void lineTo(const QPointF &endPoint, QLineF nextLine, qreal radius);
protected:

private:
	QVector<QLineF> mLines;
	
	QVector<OptimaCorner> mCorners;

	void initialize(const QPainterPath & points);

	QPointF mCurrentPosition;
};

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug, const OptimaPath &);
#endif


