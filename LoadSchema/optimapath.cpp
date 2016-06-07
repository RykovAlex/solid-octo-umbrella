#include "stdafx.h"
#include "optimapath.h"

int OptimaPath::linesCount() const
{
	return mLines.count();
}

const QLineF& OptimaPath::lineAt(int i) const
{
	return mLines.at(i);
}

int OptimaPath::cornersCount() const
{
	return mCorners.count();
}

const OptimaCorner& OptimaPath::cornerAt(int i) const
{
	return mCorners.at(i);
}

void OptimaPath::initialize(const QPainterPath & points)
{
	mLines.clear();
	mCorners.clear();

	QPointF currentPoint;
	OptimaCorner corner;
	int curveToDataElementCount = 0;

	for (int i = 0; i < points.elementCount(); ++i)
	{
		const QPainterPath::Element &element = points.elementAt(i);


		switch (element.type)
		{
		case QPainterPath::MoveToElement:
			break;
		case QPainterPath::LineToElement:
			mLines.push_back(QLineF(currentPoint, element));
			break;
		case QPainterPath::CurveToElement:		
			{
				qreal x = (element.x * 3 - currentPoint.x()) / 2;
				qreal y = (element.y * 3 - currentPoint.y()) / 2;
				corner << currentPoint << QPointF(x, y);
			}
			break;
		case QPainterPath::CurveToDataElement:
			++curveToDataElementCount;
			if (curveToDataElementCount == 2)
			{
				corner << element;
				mCorners.push_back(corner);

				corner.clear();
				curveToDataElementCount = 0;
			}
			break;
		default:
			break;
		}
		currentPoint = element;
	}
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug s, const OptimaPath &p)
{
	s.nospace() << "OptimaPath: Line count=" << p.linesCount() << endl;
	for (int i = 0; i < p.linesCount(); ++i) {
		s.nospace() << " -> " << p.lineAt(i) << endl;
	}

	s.nospace() << "OptimaPath: Corner count=" << p.cornersCount() << endl;
	for (int i = 0; i < p.cornersCount(); ++i) {
		s.nospace() << " -> " << p.cornerAt(i) << endl;
	}
	return s;
}
#endif
