#pragma once
#include "optimarectanglemarker.h"
#include "optimaconnector.h"

class OptimaConnectorLineMarker : public OptimaRectangleMarker
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

protected:


	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
	OptimaLine mLine;

	OptimaTemporaryConnector *mNewConnector;///<отображение для коннетора который получится в результате изменения текущего коннектора, владельца маркера
};


