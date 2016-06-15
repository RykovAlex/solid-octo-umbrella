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

OptimaPath::OptimaPath(const QPointF &startPoint, const OptimaCross & cross) 
	: mCurrentPosition(startPoint)
	, mCross(cross)
{

}

OptimaPath::OptimaPath(const QPointF &startPoint)
	: mCurrentPosition(startPoint)
{

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
		mLines.at(i).toPath(path, mCross);
	}
	
	path.connectPath(path.toReversed());
	
	return path;
}

void OptimaPath::intersected(OptimaPath & path)
{
	bool isSelf = this == &path;

	for (int i = 0; i < mLines.count() - (isSelf?1:0); ++i) {
		QVector<qreal> lengthVector = path.intersected(mLines.at(i), (isSelf?i+1:0));
		
		mLines[i].addIntersectedLength(lengthVector);
	}
}

QVector<qreal> OptimaPath::intersected(const OptimaLine & otherLine, int start)
{
	QVector<qreal> intersectedLength;

	for ( int i = start; i < mLines.count(); ++i )
	{
		QPointF intersectionPoint;
		
		//точка пересечения линии и линии
		if ( getIntersectionPoint(mLines.at(i), otherLine, &intersectionPoint) )
		{
			intersectedLength.push_back( getLineSublength(otherLine, intersectionPoint));	
		}
		
		//точка пересечения угда и линии
		if ( getIntersectionPoint(mLines.at(i).corner(), otherLine, &intersectionPoint) )
		{
			intersectedLength.push_back( getLineSublength(otherLine, intersectionPoint));	
		}

		//точка пересечения линии и угла
		if ( getIntersectionPoint(otherLine.corner(), mLines.at(i), &intersectionPoint) )
		{
			mLines[i].addIntersectedLength( getLineSublength(mLines.at(i), intersectionPoint));	
		}

	}

	return intersectedLength;
}

void OptimaPath::clearIntersection()
{
	for ( int i = 0; i < mLines.count(); ++i )
	{
		mLines[i].clearIntersectedLength();
	}
}

template <class T>
bool OptimaPath::getIntersectionPoint(const T & optimaObject, const OptimaLine &otherLine, QPointF *intersectionPoint) const
{
	if ( optimaObject.intersect( otherLine, intersectionPoint ) == QLineF::BoundedIntersection )
	{
		if (!otherLine.contains(*intersectionPoint))
		{
			return true;
		}
	}

	return false;
}

qreal OptimaPath::getLineSublength(const OptimaLine &line, const QPointF & point) const
{
	// определим расстояние до точки пересечения от начала отрезка
	QLineF subLine( line.p1(), point  );
	return subLine.length();
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
		s.nospace() << " line          -> " << p.lineAt(i) << endl;
		s.nospace() << " corner        -> " << p.lineAt(i).corner() << endl;
		s.nospace() << " intersections -> " << p.lineAt(i).getCrossingLength() << endl;
	}

	return s;
}
#endif
