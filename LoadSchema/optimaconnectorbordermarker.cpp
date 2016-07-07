#include "stdafx.h"
#include "optimaconnectorbordermarker.h"



OptimaConnectorBorderMarker::OptimaConnectorBorderMarker(OptimaConnector* parentConnector, const OptimaPoint & pos, Qt::CursorShape cursorShape) 
	: OptimaRectangleMarker(parentConnector, pos, cursorShape, parentConnector->view())
{

}

int OptimaConnectorBorderMarker::type() const
{
	return Type;
}

void OptimaConnectorBorderMarker::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	OptimaConnector *connector = qgraphicsitem_cast<OptimaConnector*>(parentItem());
	
	Q_ASSERT(connector != nullptr);
	connector->setRebuild(true);
}

void OptimaConnectorBorderMarker::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	//throw std::logic_error("The method or operation is not implemented.");
}

void OptimaConnectorBorderMarker::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	OptimaConnector *connector = qgraphicsitem_cast<OptimaConnector*>(parentItem());

	Q_ASSERT(connector != nullptr);
	connector->setRebuild(false);
	connector->update();
}
