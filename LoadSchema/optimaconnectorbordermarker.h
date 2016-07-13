#pragma once
#include "optimaconnector.h"

class OptimaConnectorBorderMarker : public OptimaRectangleMarker
{
	Q_DISABLE_COPY( OptimaConnectorBorderMarker );

public:
	enum { Type = UserType + tag::element::borderMarker };

	OptimaConnectorBorderMarker(OptimaConnector* parent, const OptimaPoint & pos, Qt::CursorShape cursorShape, bool reversed, bool linked);

	~OptimaConnectorBorderMarker()
	{
	}


	virtual int type() const;

protected:


	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);


private:

	bool mReversed;
};


