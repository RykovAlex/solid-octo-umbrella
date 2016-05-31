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

//	QString s = getXmlString(mNodeXml);
}

const QDomElement OptimaElement::getXmlNode(const QString & name) const
{
	return mNodeXml.namedItem( name ).toElement();
}

qreal OptimaElement::getXmlValue(const QString & name, const qreal defaultValue) const
{
	QDomNode node(mNodeXml.namedItem( name ));
	
	if (node.isNull())
	{
		return defaultValue;
	} 
	return mNodeXml.namedItem( name ).toElement().text().toDouble();
}

void OptimaElement::getXmlValue(const QString & name, QVector<OptimaPoint> &optimaPoints) const
{
	const QDomNodeList dots = getXmlNode( tag::structure_dot ).childNodes();
	if (dots.isEmpty())
	{
		return;
	}

	optimaPoints.clear();
	for ( int i = 0; i < dots.size( ); ++i )
	{
		int radius;

		optimaPoints << OptimaPoint(dots.at( i ));
	}
	optimaPoints << optimaPoints.first( );

}

void OptimaElement::getXmlValue(const QString & name, OptimaPoint &optimaPoint) const
{
	QDomNode node(mNodeXml.namedItem( name ));

	if (node.isNull())
	{
		return;
	} 

	optimaPoint.apply(node);
}

void OptimaElement::setXmlValue(const QString & name, const qreal value) const
{
	for(QDomNode n = mNodeXml.namedItem( name ).firstChild(); !n.isNull(); n = n.nextSibling())
	{
		QDomText t = n.toText();
		if (!t.isNull())
		{
			t.setNodeValue(QString::number( value ));
			break;
		}
	}
}
