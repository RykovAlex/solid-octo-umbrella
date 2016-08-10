#include "stdafx.h"
#include "optimaconnectorbordermarkerbegin.h"
#include "optimaconnector.h"
#include "optimascene.h"


OptimaConnectorBorderMarkerBegin::OptimaConnectorBorderMarkerBegin(OptimaScene *optimaScene, QPointF scenePos, OptimaConnectorBorderMarkerEnd *optimaConnectorBorderMarkerEnd) 
	: OptimaConnectorMarker(optimaScene, scenePos, Qt::SizeAllCursor)
{
	mConnectorBeginMarker = this;
	mConnectorEndMarker = optimaConnectorBorderMarkerEnd;
	mOptimaScene->addItem(this);

	setLinkedElement(mOptimaScene->getLinkedElement(mCreatePos));
}

OptimaConnectorBorderMarkerBegin::OptimaConnectorBorderMarkerBegin(OptimaConnector *connector, OptimaConnectorBorderMarkerEnd *optimaConnectorBorderMarkerEnd)
	: OptimaConnectorMarker(connector, connector->first(), Qt::SizeAllCursor, tag::undo::point_start_index + 0)
{
	mConnectorBeginMarker = this;
	mConnectorEndMarker = optimaConnectorBorderMarkerEnd;

	setLinkedElement(connector->getBorderLinking(tag::id_begin));
}
