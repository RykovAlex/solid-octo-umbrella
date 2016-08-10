#include "stdafx.h"
#include "optimaxml.h"
#include "tag.h"
#include "OptimaCross.h"
#include "optimaconnectorarrow.h"

OptimaXml::OptimaXml()
{

}

OptimaXml::~OptimaXml()
{

}

QDomElement OptimaXml::nodeXml(QDomNode node, const QString & name)
{
	return node.namedItem( name ).toElement();
}

void OptimaXml::setXmlValue(QDomNode & node, const OptimaPointVector &points, const QString &ownerId)
{
	QDomElement nodeElement = nodeXml( node, tag::structure_dot );
	while (nodeElement.childNodes().count() > 0)
	{
		nodeElement.removeChild(nodeElement.firstChild());
	}

	const QDomNodeList dots = nodeXml( node, tag::structure_dot ).childNodes();

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

void OptimaXml::setXmlValue(const OptimaPointVector &points, const QString &ownerId)
{
	setXmlValue(mNodeXml, points, ownerId);
}

void OptimaXml::setXmlValue(QDomNode & node, const QString & name, const QString &value)
{
	QDomElement nodeElement = nodeXml( node, name );
	while (nodeElement.childNodes().count() > 0)
	{
		nodeElement.removeChild(nodeElement.firstChild());
	}
	QDomDocument domDocument = node.toDocument();
	nodeElement.appendChild( domDocument.createTextNode( value ) );
}

qreal OptimaXml::getXmlValue(const QString & name, const qreal defaultValue) const
{
	QDomElement node( nodeXml( name ) );

	if (node.isNull())
	{
		return defaultValue;
	} 
	return node.text().toDouble();
}

QColor OptimaXml::getXmlValue(const QString & name, const QColor defaultColor) const
{
	QDomElement node(nodeXml( name ));

	if (node.isNull())
	{
		return defaultColor;
	} 

	QColor resultColor( defaultColor );

	const QString colorName = node.text( );
	if ( colorName.size( ) == 6 )
	{
		resultColor = QColor( "#" + node.text( ) );
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

QString OptimaXml::getXmlValue(const QString & name, const QString &defaultString) const
{
	QDomElement node(nodeXml( name ));

	if (node.isNull())
	{
		return defaultString;
	} 

	return node.text();
}

const QFont OptimaXml::getXmlValue(const QString & name, const QFont &defaultFont) const
{
	QDomElement node(nodeXml(tag::name_font ));

	if (node.isNull())
	{
		return defaultFont;
	}

	QFont font(node.text());
	font.setPointSize(getXmlValue(tag::size_font, 10.0));
	font.setBold(getXmlValue(tag::bold_font, 0.0));
	font.setItalic(getXmlValue(tag::italic_font, 0.0));
	font.setUnderline(getXmlValue(tag::underline_font, 0.0));
	font.setStrikeOut(getXmlValue(tag::strikethru_font, 0.0));


	return font;
}

void OptimaXml::getXmlValue(const QString & name, OptimaPointVector &optimaPoints) const
{
	const QDomNodeList dots = nodeXml( tag::structure_dot ).childNodes();
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

void OptimaXml::getXmlValue(const QString & name, OptimaPoint &optimaPoint) const
{
	QDomNode node(nodeXml( name ));

	if (node.isNull())
	{
		return;
	} 

	optimaPoint.apply(node);
}

void OptimaXml::getXmlValue(const QString & name, OptimaConnectorArrow &optimaConnectorArrow) const
{
	QDomElement node(nodeXml( name ));

	if (node.isNull())
	{
		return;
	} 

	optimaConnectorArrow.apply(node.text());
}

void OptimaXml::getXmlValue(const QString & name, OptimaCross &optimaCrossing) const
{
	QDomElement node(nodeXml( name ));

	if (node.isNull())
	{
		return;
	} 

	optimaCrossing.apply(node.text());
}

Qt::PenStyle OptimaXml::getXmlValue(const QString & name, const Qt::PenStyle defaultStyle) const
{
	QDomElement node(nodeXml( name ));

	if (node.isNull())
	{
		return defaultStyle;
	} 

	const QString borderStyle = node.text( );
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

const QDomElement OptimaXml::nodeXml(const QString & name) const
{
	QDomNode node( mNodeXml.namedItem( name ) );

	if (node.isNull())
	{
		node = mNodePatternXml.namedItem( name );
	} 

	return node.toElement();
}

void OptimaXml::setXmlValue(const QString & name, const qreal value) 
{
	OptimaXml::setXmlValue(mNodeXml, name, value);
}

void OptimaXml::setXmlValue(const QString & name, const QString &value) 
{
	OptimaXml::setXmlValue(mNodeXml, name, value);
}

void OptimaXml::setXmlValue(QDomNode & node, const QString & name, const qreal value)
{
	setXmlValue(node, name, QString::number(value));
}

void OptimaXml::updateXml(const QDomNode &element)
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


void OptimaXml::applyXml(const QDomNode & element, const QDomNode & elementPattern)
{
	//шаблон объекта всегда просто сохраняем
	mNodePatternXml = elementPattern.cloneNode();
	
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
	
	apply();
}

QString OptimaXml::getXmlString(const QDomNode & element)
{
	QString s;
	QTextStream figureText(&s);
	element.save(figureText, 4);

	return s;
}

QString OptimaXml::getXmlString()
{
	return getXmlString(mNodeXml);
}

