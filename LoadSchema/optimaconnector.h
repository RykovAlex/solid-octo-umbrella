#pragma once
#include "optimaelement.h"
#include <QtXml/QDOMDocument>
#include "optimapoint.h"
#include "optimaconnectorarrow.h"

class OptimaConnector : public QGraphicsPathItem, public OptimaElement
{
public:
	OptimaConnector(const QString &itemUuid);

	~OptimaConnector()
	{
	}
	
	///ѕрименить изенени€ переданне через струтуру xml
	void apply(const QDomNode & connector);

protected:

private:
	OptimaPointVector mPoints;///< действительные точки фигуры, дл€ отрисовке на сцене

	OptimaConnectorArrow mBeginArrow;///<—трелка в начале коннектора

	OptimaConnectorArrow mEndArrow;///<—трелка в конце коннектора

	void draw();

	const QPointF & drawArrow(QPainterPath pp_arrows, UnknownType param2, UnknownType param3, OptimaConnectorArrow beginArrow, bool param5);
};


