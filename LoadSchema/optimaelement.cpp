#include "stdafx.h"
#include "tag.h"
#include "optimaelement.h"

OptimaElement::OptimaElement(QGraphicsItem *_item) : item(_item)
{

}

QString OptimaElement::getXml(const QDomNode & element)
{
	QString s;
	QTextStream figureText(&s);
	element.save(figureText, 4);

	return s;
}

QPointF OptimaElement::convert( const QString & text, int & radius /*= 0*/ )
{
	if ( text.isEmpty( ) )
	{
			throw QObject::tr( "Ошибка анализа координаты, пустая координата" );
	}
	
	const QStringList pointsList = text.split( ":" );
	if ( pointsList.size( ) < 2 )
	{
		throw QObject::tr( "Ошибка анализа координаты, недостает параметров" );
	}

	// если 2 элемента, то это просто угол, если 3 то третий параметр это радиус
	if ( pointsList.size( ) >= 3 )
	{
		radius = pointsList.at( 2 ).toInt( );
	}

	return QPointF( pointsList.at( 0 ).toDouble( ), pointsList.at( 1 ).toDouble( ) );
}

void OptimaElement::applyXml(const QDomNode & element)
{
	//если объекта нет, то проинициализируем его переданым
	if (nodeXml.isNull())
	{
		nodeXml = element.cloneNode();
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

			QDomNode oldNode = nodeXml.namedItem(tagName);
			if(oldNode.isNull())
			{
				nodeXml.appendChild(newNode.cloneNode());
			}
			else
			{
				nodeXml.replaceChild(newNode.cloneNode(), oldNode);
			}
			
		}
		newNode = newNode.nextSibling();	
	}

//	QString s = getXml(nodeXml);
}

