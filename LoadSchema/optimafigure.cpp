#include "stdafx.h"
#include "tag.h"
#include "optimafigure.h"

OptimaFigure::OptimaFigure(const QString &itemUuid) : OptimaElement(this)
{
	QPainterPath path( QPointF(0,0));
	path.lineTo(0,100);
	path.lineTo(100,100);
	path.lineTo(100,0);
	path.lineTo(0,0);
	setPath( path );
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
	
	setData(tag::data::uuid, itemUuid);
}

void OptimaFigure::apply(const QDomNode & figure)
{
	applyXml(figure);
}

void OptimaFigure::applyXml(const QDomNode & figure)
{
	//Получим существующий xml - описатель объекта
	QString xml = data(tag::data::xml).toString();
	
	//Если его нет, то все просто, присваиваем новый
	if (xml.isEmpty())
	{
		xml = getXml(figure);
		setData(tag::data::xml, xml);

		return;
	}
	
	//xml - описатель есть, значит нужно внести в него изменения
	QDomDocument doc;
	Q_ASSERT( doc.setContent( xml ));
	QDomElement d = doc.documentElement();

	QDomNode newNode = figure.firstChild();
	while (!newNode.isNull()) 
	{
		if (newNode.isElement()) 
		{
			QString tagName = newNode.toElement().tagName();

			QDomNode oldNode = d.namedItem(tagName);
			Q_ASSERT( !oldNode.isNull() );

			d.replaceChild(newNode.cloneNode(), oldNode);
		}
		newNode = newNode.nextSibling();
		
	}

	xml = getXml(d);
	setData(tag::data::xml, xml);
}
