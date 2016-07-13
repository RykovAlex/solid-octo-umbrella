#include "stdafx.h"
#include "optimaconnectorbordermarker.h"



OptimaConnectorBorderMarker::OptimaConnectorBorderMarker(OptimaConnector* parent, const OptimaPoint & pos, Qt::CursorShape cursorShape, bool reversed, bool linked)
	: OptimaRectangleMarker(parent, pos, cursorShape, parent->view())
	, mReversed(reversed)
{
	setLinked(linked);
}

int OptimaConnectorBorderMarker::type() const
{
	return Type;
}

void OptimaConnectorBorderMarker::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	OptimaConnector *connector = qgraphicsitem_cast<OptimaConnector*>(parentItem());
	
	Q_ASSERT(connector != nullptr);
	connector->setRebuild(true, mReversed);
}