#include "stdafx.h"
#include "tag.h"
#include "optimaelement.h"
#include "optimaview.h"

#define Z_ORDER_DELIMITER 10000.0

OptimaElement::OptimaElement(QGraphicsItem *_item, const QString &itemUuid, OptimaView *view) 
	: mItem(_item)
	, mView(view)
{
	mItem->setData(tag::data::uuid, itemUuid);
}

OptimaElement::OptimaElement() 
	: mItem(nullptr)
	, mView(nullptr)
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
	//���� ������� ���, �� ����������������� ��� ���������
	if (mNodeXml.isNull())
	{
		mNodeXml = element.cloneNode();
	}
	else
	{
		//xml - ��������� ����, ������ ����� ������ � ���� ���������
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
	//���� ��� ������������ �������, ��������������� �� ����� ������, �� ������ �� ���������
	if (mItem == nullptr)
	{
		return;
	}

	//������ ������� ����������� �� �����
	qreal zOrder = getXmlValue(tag::order, 1.0);
	mItem->setZValue( ( zOrder >= 0.0 && zOrder <= Z_ORDER_DELIMITER ) ? zOrder / Z_ORDER_DELIMITER : 1.0 );

	//������ ������������ ����
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

	//������ ���� � ������� ��� ���, �� �������, � ����������� �� ������ ��������
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

void OptimaElement::onMarkerMove(const OptimaBaseMarker* marker)
{
	throw std::logic_error("The method or operation is not implemented.");
}

const QDomElement OptimaElement::getXmlNode(const QString & name) const
{
	return mNodeXml.namedItem( name ).toElement();
}

QDomElement OptimaElement::getXmlNode(QDomNode node, const QString & name)
{
	return node.namedItem( name ).toElement();
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

void OptimaElement::setXmlValue(const QString & name, const qreal value) 
{
	OptimaElement::setXmlValue(mNodeXml, name, value);
}

void OptimaElement::setXmlValue(const QString & name, const QString &value) 
{
	OptimaElement::setXmlValue(mNodeXml, name, value);
}

void OptimaElement::setXmlValue(QDomNode & node, const OptimaPointVector &points, const QString &ownerId)
{
	QDomElement nodeElement = getXmlNode( node, tag::structure_dot );
	while (nodeElement.childNodes().count() > 0)
	{
		nodeElement.removeChild(nodeElement.firstChild());
	}

	const QDomNodeList dots = getXmlNode( node, tag::structure_dot ).childNodes();
	
	Q_ASSERT(dots.isEmpty());
	
	foreach( const OptimaPoint &p, points)
	{
		QDomDocument domDocument = node.toDocument();
		QDomElement domElement = domDocument.createElement( "dot" );
		nodeElement.appendChild( domElement );
		domElement.appendChild( tag::createNode( domDocument, tag::id, p.uuid()) );
		domElement.appendChild( tag::createNode( domDocument, tag::id_owner, ownerId ) );
		domElement.appendChild( tag::convertFromPoint( domDocument, "Coordinate", p ) );
	}
}

void OptimaElement::setXmlValue(QDomNode & node, const QString & name, const QString &value)
{
	QDomElement nodeElement = getXmlNode( node, name );
	while (nodeElement.childNodes().count() > 0)
	{
		nodeElement.removeChild(nodeElement.firstChild());
	}
	QDomDocument domDocument = node.toDocument();
	nodeElement.appendChild( domDocument.createTextNode( value ) );
}

void OptimaElement::setXmlValue(QDomNode & node, const QString & name, const qreal value)
{
	setXmlValue(node, name, QString::number(value));
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

QString OptimaElement::uuid() const
{
	return mItem->data(tag::data::uuid).toString();
}

void OptimaElement::setXmlZOrder(QDomNode & node, qreal sceneZOrder)
{
	int xmlZOrder = static_cast< int >( qRound(sceneZOrder * Z_ORDER_DELIMITER));

	setXmlValue(node, tag::order, xmlZOrder);
}
