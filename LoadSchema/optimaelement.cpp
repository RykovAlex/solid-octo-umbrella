#include "stdafx.h"
#include "tag.h"
#include "optimaelement.h"

OptimaElement::OptimaElement(QGraphicsItem *_item) : mItem(_item)
{

}

QString OptimaElement::getXmlString(const QDomNode & element) const
{
	QString s;
	QTextStream figureText(&s);
	element.save(figureText, 4);

	return s;
}

void OptimaElement::applyXml(const QDomNode & element)
{
	//если объекта нет, то проинициализируем его переданым
	if (mNodeXml.isNull())
	{
		mNodeXml = element.cloneNode();
	}
	else
	{
		//xml - описатель есть, значит нужно внести в него изменения
		updateXml(element);
	}
}

void OptimaElement::updateXml(const QDomNode &element)
{
	QDomNode newNode = element.firstChild();
	while (!newNode.isNull()) 
	{
		if (newNode.isElement()) 
		{
			QString tagName = newNode.toElement().tagName();

			QDomNode oldNode = mNodeXml.namedItem(tagName);
			if(oldNode.isNull())
			{
				mNodeXml.appendChild(newNode.cloneNode());
			}
			else
			{
				mNodeXml.replaceChild(newNode.cloneNode(), oldNode);
			}
			
		}
		newNode = newNode.nextSibling();	
	}
	
	QString::number( value )
	QString s = getXml(nodeXml);
}

const QDomElement OptimaElement::getXmlNode(const QString & name) const
{
	return mNodeXml.namedItem( name ).toElement();
}

qreal OptimaElement::getXmlRealValue(const QString & name) const
{
	mNodeXml.namedItem( name ).toElement().text().toDouble();
}

void OptimaElement::setXmlValue(const QString & name, const qreal value) const
{
	mNodeXml.namedItem( name ).toElement().setNodeValue(QString::number( value ));
}
