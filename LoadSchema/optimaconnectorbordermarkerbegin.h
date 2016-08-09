#pragma once
#include "QPoint"
#include "optimarectanglemarker.h"
#include "OptimaConnectorMarker.h"

class OptimaScene;
class OptimaConnectorBorderMarkerEnd;
class OptimaConnectorBorderMarkerBegin: public OptimaConnectorMarker
{
	Q_DISABLE_COPY( OptimaConnectorBorderMarkerBegin );

public:
	enum { Type = UserType + tag::element::borderMarkerBegin };

	OptimaConnectorBorderMarkerBegin(OptimaScene *optimaScene, QPointF scenePos, OptimaConnectorBorderMarkerEnd *optimaConnectorBorderMarkerEnd);

	OptimaConnectorBorderMarkerBegin(OptimaConnector *connector, OptimaConnectorBorderMarkerEnd *optimaConnectorBorderMarkerEnd);

	~OptimaConnectorBorderMarkerBegin()
	{
	}

protected:

private:
};


