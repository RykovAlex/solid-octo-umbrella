#include "stdafx.h"
#include "tag.h"
#include "optimaelement.h"

OptimaElement::OptimaElement(QGraphicsItem *_item, const QString &itemUuid) : mItem(_item)
{
	mItem->setData(tag::data::uuid, itemUuid);
}

OptimaElement::OptimaElement() : mItem(nullptr)
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

	applyCommonProperties();

	apply();
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

void OptimaElement::applyCommonProperties()
{
	//Если нет гравического элемнта, унаследованного от этого класса, то ничего не загружаем
	if (mItem == nullptr)
	{
		return;
	}

	//Задаем порядок отображеняи на схеме
	mItem->setZValue(getXmlValue(tag::order, 1.0));

	//Задаем отбрасывание тени
	if ( getXmlValue(tag::drop_shadow, 0.0) != 0 )
	{
		QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();	
		effect->setBlurRadius(10.0);
		mItem->setGraphicsEffect( effect );
	}
	else
	{
		mItem->setGraphicsEffect( nullptr );
	}	

	//Задаем цвет и заливку или фон, по разному, в зависимости от разных объектов
	QGraphicsPathItem *pathItem = dynamic_cast<QGraphicsPathItem*>(mItem);
	QGraphicsTextItem *textItem = dynamic_cast<QGraphicsTextItem*>(mItem);

	if ( pathItem != nullptr )
	{
		QPen pen;
		pen.setColor( getXmlValue(tag::paint_line, QColor(Qt::black)) );
		pen.setWidth( getXmlValue(tag::thickness_line, 1.0));
		pen.setStyle( getXmlValue(tag::type_line, Qt::SolidLine));
		pen.setCosmetic( true );

		pathItem->setPen(pen);
	}
	else
	if ( textItem != nullptr )
	{
	}
	
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

void OptimaElement::getXmlValue(const QString & name, OptimaPointVector &optimaPoints) const
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

void OptimaElement::getXmlValue(const QString & name, OptimaConnectorArrow &optimaConnectorArrow) const
{
	QDomNode node(mNodeXml.namedItem( name ));

	if (node.isNull())
	{
		return;
	} 

	optimaConnectorArrow.apply(node.toElement().text());
}

void OptimaElement::getXmlValue(const QString & name, OptimaCross &optimaCrossing) const
{
	QDomNode node(mNodeXml.namedItem( name ));

	if (node.isNull())
	{
		return;
	} 

	optimaCrossing.apply(node.toElement().text());
}

Qt::PenStyle OptimaElement::getXmlValue(const QString & name, const Qt::PenStyle defaultStyle) const
{
	QDomNode node(mNodeXml.namedItem( name ));

	if (node.isNull())
	{
		return defaultStyle;
	} 

	const QString borderStyle = node.toElement( ).text( );
	if ( borderStyle == "dot" )
		return Qt::DotLine;
	else if ( borderStyle == "dash" )
		return Qt::DashLine;
	else if ( borderStyle == "dotdash" )
		return Qt::DashDotLine;
	else if ( borderStyle == "no" )
		return Qt::NoPen;
	else
		return Qt::SolidLine;

}

QString OptimaElement::getXmlValue(const QString & name, const QString &defaultString) const
{
	QDomNode node(mNodeXml.namedItem( name ));

	if (node.isNull())
	{
		return defaultString;
	} 

	return node.toElement( ).text( );
}

const QFont OptimaElement::getXmlValue(const QString & name, const QFont &defaultFont) const
{
	QDomNode node(mNodeXml.namedItem( tag::name_font ));
	
	if (node.isNull())
	{
		return defaultFont;
	}

	QFont font(node.toElement( ).text());
	font.setPointSize(getXmlValue(tag::size_font, 10.0));
	font.setBold(getXmlValue(tag::bold_font, 0.0));
	font.setItalic(getXmlValue(tag::italic_font, 0.0));
	font.setUnderline(getXmlValue(tag::underline_font, 0.0));
	font.setStrikeOut(getXmlValue(tag::strikethru_font, 0.0));


	return font;
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

QColor OptimaElement::getXmlValue(const QString & name, const QColor defaultColor) const
{
	QDomNode node(mNodeXml.namedItem( name ));

	if (node.isNull())
	{
		return defaultColor;
	} 

	QColor resultColor( defaultColor );

	const QString colorName = node.toElement( ).text( );
	if ( colorName.size( ) == 6 )
	{
		resultColor = QColor( "#" + node.toElement( ).text( ) );
	}
	else 
	if ( colorName.size( ) == 8 )
	{
		bool ok1, ok2, ok3, ok4;
		resultColor = QColor( static_cast< unsigned char >( colorName.mid( 0, 2 ).toInt( &ok1, 16 ) )
			, static_cast< unsigned char >( colorName.mid( 2, 2 ).toInt( &ok2, 16 ) )
			, static_cast< unsigned char >( colorName.mid( 4, 2 ).toInt( &ok3, 16 ) )
			, static_cast< unsigned char >( colorName.mid( 6, 2 ).toInt( &ok4, 16 ) ) );
	}

	return resultColor.isValid( ) ? resultColor : Qt::gray;
}
