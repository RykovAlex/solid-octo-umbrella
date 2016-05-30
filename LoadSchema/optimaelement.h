#pragma once
#include <QtXml/QDOMDocument>

class OptimaElement
{
private:
	QGraphicsItem *item;

protected:	
	QString getXml(const QDomNode & figure);

public:
	OptimaElement(QGraphicsItem *_item);
	

	~OptimaElement()
	{
	}

protected:

};


