#pragma once
#include "optimarectanglemarker.h"
#include "optimaconnector.h"

/// маркер между точками коннектора
class OptimaConnectorMoveMarker : public OptimaRectangleMarker
{
	Q_DISABLE_COPY( OptimaConnectorMoveMarker );

public:
	OptimaConnectorMoveMarker( OptimaConnector *parent, const QPointF & pos, Qt::CursorShape cursorShape);
	
	~OptimaConnectorMoveMarker()
	{
	}

	virtual bool isPosChanged( QPointF & pos );
	

protected:


	virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:

};


