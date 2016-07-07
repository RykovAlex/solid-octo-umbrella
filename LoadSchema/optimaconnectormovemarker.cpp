#include "stdafx.h"
#include "optimaconnectormovemarker.h"

OptimaConnectorMoveMarker::OptimaConnectorMoveMarker(OptimaConnector *parentConnector, const QPointF & pos, Qt::CursorShape cursorShape) 
	: OptimaRectangleMarker(parentConnector, pos, cursorShape, parentConnector->view())
{
}

int OptimaConnectorMoveMarker::type() const
{
	return Type;
}
