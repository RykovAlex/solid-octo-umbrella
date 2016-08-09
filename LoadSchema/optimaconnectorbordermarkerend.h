#pragma once
#include "OptimaConnectorMarker.h"

class OptimaConnectorBorderMarkerEnd : public OptimaConnectorMarker
{
	Q_DISABLE_COPY( OptimaConnectorBorderMarkerEnd );
public:
	enum { Type = UserType + tag::element::borderMarkerEnd };

	OptimaConnectorBorderMarkerEnd(OptimaScene* optimaScene, QPointF scenePos);

	OptimaConnectorBorderMarkerEnd(OptimaConnector* connector);

	~OptimaConnectorBorderMarkerEnd();

protected:

private:


};


