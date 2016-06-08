#include "stdafx.h"
#include "optimapath.h"
#include "optimaline.h"

OptimaPath::OptimaPath()
{
}

OptimaPath::OptimaPath(const QPainterPath &points)
{
	initialize(points);
}

int OptimaPath::linesCount() const
{
	return mLines.count();
}

const OptimaLine & OptimaPath::lineAt(int i) const
{
	return mLines.at(i);
}

const OptimaCorner& OptimaPath::cornerAt(int i) const
{
	return mLines.at(i).corner();
}

void OptimaPath::lineTo(const QPointF &endPoint)
{
	mLines.push_back(OptimaLine(mCurrentPosition, endPoint));
	mCurrentPosition = endPoint;
}

const QPainterPath OptimaPath::toPath() const
{
	QPainterPath path;
	for (int i = 0; i < mLines.count(); ++i) {
		mLines.at(i).toPath(path);
	}

	return path;
}

void OptimaPath::intersected(const OptimaPath path)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void OptimaPath::lineTo(const QPointF &endPoint, QLineF nextLine, qreal radius)
{
	OptimaLine firstLine(mCurrentPosition, endPoint);

	//проверим что радиус скругления не больше чем размер отрезка, в противном случае уменьшим радиус
	//скругления до половины размера отрезка и применим этот радиус также и ко второму отрезку
	qreal len = std::min(firstLine.length(), nextLine.length());

	if ( len < radius * 2. )
	{
		radius = len / 2.;
	}

	firstLine.setLength( firstLine.length() - radius );
	nextLine.setLength( nextLine.length() - radius );
	firstLine.setCorner(firstLine.p2(), endPoint, nextLine.p2());
	
	mLines.push_back(firstLine);

	mCurrentPosition = nextLine.p2();
}

void OptimaPath::initialize(const QPainterPath & points)
{
	mLines.clear();

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
			mLines.push_back(OptimaLine(currentPoint, element));
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
				mLines.last().setCorner(corner);

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
	s.nospace() << "OptimaPath: Optima line count=" << p.linesCount() << endl;
	for (int i = 0; i < p.linesCount(); ++i) {
		s.nospace() << " line   -> " << p.lineAt(i) << endl;
		s.nospace() << " corner -> " << p.lineAt(i).corner() << endl;
	}

	return s;
}
#endif
