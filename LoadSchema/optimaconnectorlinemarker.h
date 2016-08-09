#pragma once
#include "OptimaConnectorMarker.h"

class OptimaConnectorMarker;
class OptimaConnectorLineMarker : public OptimaConnectorMarker
{
	Q_DISABLE_COPY( OptimaConnectorLineMarker );

public:	
	enum { Type = UserType + tag::element::lineMarker };

	OptimaConnectorLineMarker(OptimaConnector * parentConnector, const OptimaLine &line, Qt::CursorShape cursorShape);

	~OptimaConnectorLineMarker()
	{
	}

	int getIndexLine() const;

	OptimaPoint p1() const;

	OptimaPoint p2() const;


	virtual void setMarkerPos(const QPointF & pos);

	void setNewConnectorPoints();

protected:

private:
	OptimaLine mLine;

	OptimaPointVector getNewPoints();
};


