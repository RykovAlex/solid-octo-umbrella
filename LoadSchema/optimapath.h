#pragma once
#include "optimapoint.h"
#include "optimacorner.h"
#include "optimaline.h"

class OptimaPath
{
public:
	OptimaPath();

	OptimaPath(const QPainterPath &points);

	OptimaPath(const QPointF &startPoint): mCurrentPosition(startPoint)
	{
	}

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



	void intersected(const OptimaPath path);
protected:



private:

	QVector<OptimaLine> mLines;
	
	QPointF mCurrentPosition;

	void initialize(const QPainterPath & points);
};

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug, const OptimaPath &);
#endif


