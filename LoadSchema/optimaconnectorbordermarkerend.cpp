#include "stdafx.h"
#include "tag.h"
#include "optimascene.h"
#include "optimaconnectorbordermarkerend.h"
#include "optimaconnectorbordermarkerbegin.h"
#include "optimaconnector.h"

OptimaConnectorBorderMarkerEnd::OptimaConnectorBorderMarkerEnd(OptimaScene* optimaScene, QPointF scenePos) 
	: OptimaConnectorMarker(optimaScene, scenePos, Qt::SizeAllCursor)
{
	mOptimaScene->addItem(this);
	mConnectorBeginMarker =  new OptimaConnectorBorderMarkerBegin(optimaScene, scenePos, this);
	
	mConnectorBeginMarker->updateLinkedHighlight();

	mConnectorEndMarker = this;

	setLinkedElement(mOptimaScene->getLinkedElement(mCreatePos));
}

OptimaConnectorBorderMarkerEnd::OptimaConnectorBorderMarkerEnd(OptimaConnector* connector)
	: OptimaConnectorMarker(connector, connector->last(), Qt::SizeAllCursor)
{
	mConnectorBeginMarker =  new OptimaConnectorBorderMarkerBegin(connector, this);
	mConnectorEndMarker = this;

	setLinkedElement(connector->getBorderLinking(tag::id_end));
}

OptimaConnectorBorderMarkerEnd::~OptimaConnectorBorderMarkerEnd()
{
	delete mConnectorBeginMarker;
}
