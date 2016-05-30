#include "stdafx.h"
#include "optimaelement.h"

OptimaElement::OptimaElement(QGraphicsItem *_item) : item(_item)
{

}

QString OptimaElement::getXml(const QDomNode & figure)
{
	QString s;
	QTextStream figureText(&s);
	figure.save(figureText, 4);

	return s;
}


