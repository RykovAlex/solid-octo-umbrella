#include "stdafx.h"
#include "optimaline.h"

OptimaLine::OptimaLine(const QPointF &pt1, const QPointF &pt2) :
QLineF(pt1, pt2)
{

}

OptimaLine::OptimaLine(const QLineF line, const QVector<QString> uuids) 
	: QLineF(line)
	, mUuids(uuids)
{
	Q_ASSERT(mUuids.size() == 2);
}

OptimaLine::OptimaLine()
{

}

void OptimaLine::setCorner(const QPointF &beginPoint, const QPointF &cornerPoint, const QPointF &endPoint)
{
	mCorner = OptimaCorner(beginPoint, cornerPoint, endPoint);
}

void OptimaLine::setCorner(const OptimaCorner &corner)
{
	mCorner = corner;
}

const OptimaCorner & OptimaLine::corner() const
{
	return mCorner; 
}

void OptimaLine::toPath(QPainterPath &path, const OptimaCross & cross) const
{
	if (path.currentPosition() != p1())
	{
		path.moveTo(p1());
	}	
	
	cross.draw(path, *this, mCrossingWithConnectorLengths);
	
	path.lineTo(p2());

	if (mCorner.isEmpty())
	{
		return;
	}

	Q_ASSERT(mCorner.count() == 3);

	path.quadTo(mCorner.at(1), mCorner.at(2));
}

void OptimaLine::addIntersectedLength(const QVector<qreal> & intersectedLengths)
{
	mCrossingWithConnectorLengths << intersectedLengths;
}

void OptimaLine::addIntersectedLength(qreal intersectedLength)
{
	mCrossingWithConnectorLengths << intersectedLength;
}

void OptimaLine::clearIntersectedLength()
{
	mCrossingWithConnectorLengths.clear();
}

bool OptimaLine::contains(const QPointF & intersectionPoint) const
{
	return p1() == intersectionPoint || p2() == intersectionPoint;
}

QVector<qreal> OptimaLine::getCrossingLength() const
{
	return mCrossingWithConnectorLengths;
}

void OptimaLine::translate(const QPointF & offset)
{
	QLineF::translate(offset);

	mCorner.translate(offset);
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug s, const OptimaLine &p)
{
	s.nospace() << "OptimaLine:" << endl;
	s.nospace() << "    " << QLineF(p) << endl;
	s.nospace() << "    uuids( " << p.uuid1() << ", " << p.uuid2() << " )" << endl;

	return s;
}
#endif
