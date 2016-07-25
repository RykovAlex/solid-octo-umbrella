#include "stdafx.h"
#include "optimaconnectorlinemarker.h"

OptimaConnectorLineMarker::OptimaConnectorLineMarker(OptimaConnector * parentConnector, const OptimaLine &line, Qt::CursorShape cursorShape) 
	: OptimaRectangleMarker(parentConnector, line.center(), cursorShape, parentConnector->view())
	, mLine(line)
{

}

int OptimaConnectorLineMarker::getIndexLine() const
{
	return mLine.getIndexLine();
}

OptimaPoint OptimaConnectorLineMarker::p1() const
{
	return mLine[OptimaLine::point1];
}

OptimaPoint OptimaConnectorLineMarker::p2() const
{
	return mLine[OptimaLine::point2];
}

void OptimaConnectorLineMarker::setMarkerPos(const QPointF & pos)
{
	switch(mCursor.shape())
	{
	case Qt::SizeHorCursor:
		mLine.setP1(QPointF(pos.x(), mLine.p1().y()));
		mLine.setP2(QPointF(pos.x(), mLine.p2().y()));
		break;
	case Qt::SizeVerCursor:
		mLine.setP1(QPointF(mLine.p1().x(), pos.y()));
		mLine.setP2(QPointF(mLine.p2().x(), pos.y()));
		break;
	default:
		Q_ASSERT( false );
	}
	setPos(pos);

	OptimaPointVector newPoints;

	//теперь можно узнать какую функцию надо вызвать для оперделения пути коннектора от
	//начальной к конечной точке
	switch(mNewConnector->getRelationship())
	{
	case OptimaTemporaryConnector::free_free:
		newPoints = OptimaConnectorPathFinder::GetLineConnectorPoints_Free_Free( mNewConnector->points(), getIndexLine(), mLine);
		break;
	}

	mNewConnector->setPoints(newPoints);
}

void OptimaConnectorLineMarker::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	OptimaConnector* parentConnector = dynamic_cast<OptimaConnector*>(parentItem());
	mNewConnector = new OptimaTemporaryConnector(parentConnector);

	OptimaRectangleMarker::mousePressEvent(event);
}

void OptimaConnectorLineMarker::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	//здесь нужно прибить временный коннектор и передать на основной коннектор новые координаты
	delete mNewConnector;
}
