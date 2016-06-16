#pragma once
#include "optimarectanglemarker.h"
#include "optimaview.h"

/// маркер между точками коннектора
class OptimaConnectorMoveMarker : public OptimaRectangleMarker
{
	Q_DISABLE_COPY( OptimaConnectorMoveMarker );

public:
	OptimaConnectorMoveMarker( Qt::CursorShape cursorShape, const OptimaView *view );
	
	~OptimaConnectorMoveMarker()
	{
	}

	virtual bool isPosChanged( QPointF & pos );
	

protected:

private:

};


