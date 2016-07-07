#pragma once
#include "optimarectanglemarker.h"
#include "optimaconnector.h"

/// маркер между точками коннектора
class OptimaConnectorMoveMarker : public OptimaRectangleMarker
{
	Q_DISABLE_COPY( OptimaConnectorMoveMarker );

public:
	enum { Type = UserType + tag::element::moveMarker };

	OptimaConnectorMoveMarker( OptimaConnector *parent, const QPointF & pos, Qt::CursorShape cursorShape);
	
	~OptimaConnectorMoveMarker()
	{
	}


	virtual int type() const;

protected:

private:

};


