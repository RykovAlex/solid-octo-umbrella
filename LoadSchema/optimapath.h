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

	~OptimaPath()
	{
	}

	int linesCount() const;
	const QLineF& lineAt(int i) const;
	int cornersCount() const;
	const OptimaCorner& cornerAt(int i) const;
protected:

private:
	QVector<QLineF> mLines;
	
	QVector<OptimaCorner> mCorners;

	void initialize(const QPainterPath & points);

};

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug, const OptimaPath &);
#endif


