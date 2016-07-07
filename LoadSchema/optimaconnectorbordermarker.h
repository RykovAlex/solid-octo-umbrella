#pragma once
#include "optimaconnector.h"

class OptimaConnectorBorderMarker : public OptimaRectangleMarker
{
	Q_DISABLE_COPY( OptimaConnectorBorderMarker );

public:
	enum { Type = UserType + tag::element::borderMarker };

	OptimaConnectorBorderMarker(OptimaConnector* parent, const OptimaPoint & pos, Qt::CursorShape cursorShape);

	~OptimaConnectorBorderMarker()
	{
	}


	virtual int type() const;

protected:


	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);


	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);


	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:

};


