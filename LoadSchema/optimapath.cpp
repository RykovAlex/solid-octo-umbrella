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

void OptimaPath::lineTo(const QPointF &endPoint)
{
	mLines.push_back(QLineF(mCurrentPosition, endPoint));
	mCurrentPosition = endPoint;
}

qreal OptimaPath::getCornerRadius(const QLineF &line1, const QLineF &line2, qreal radius) const
{
	qreal radius( mRadiusCorner );

	const QLineF line1( mPoints.at( indexCorner - 1 ), mPoints.at( indexCorner ) );
	const QLineF line2( mPoints.at( indexCorner + 1), mPoints.at( indexCorner ) );

	qreal len = std::min( line1.length(), line2.length() );

	if ( len < radius * 2. )
	{
		radius = len / 2.;
	}

	return radius;

}

void OptimaPath::lineTo(const QPointF &endPoint, QLineF nextLine, qreal radius)
{
	QLineF firstLine(mCurrentPosition, endPoint);

	//проверим что радиус скругления не больше чем размер отрезка, в противном случае уменьшим радиус
	//скругления до половины размера отрезка и применим этот радиус также и ко второму отрезку
	qreal len = std::min(firstLine.length(), nextLine.length());

	if ( len < radius * 2. )
	{
		radius = len / 2.;
	}

	firstLine.setLength( firstLine.length() - radius );
	nextLine.setLength( nextLine.length() - radius );

	mLines.push_back(firstLine);
	
	mCorners.push_back(OptimaCorner(firstLine.p2(), endPoint, nextLine.p2()));
	
	mCurrentPosition = nextLine.p2();
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
