#pragma once
#include "optimaelement.h"
#include <QtXml/QDOMDocument>

class OptimaFigure : public QGraphicsPathItem, public OptimaElement
{
public:
	OptimaFigure(const QString &itemUuid);

	~OptimaFigure()
	{
	}

protected:

public:
	void apply(const QDomNode & figure);
};


