#include "stdafx.h"
#include "tag.h"
#include "optimaelement.h"
#include "optimascene.h"

#define Z_ORDER_DELIMITER 10000.0

OptimaElement::OptimaElement(QGraphicsItem *_item, const QString &itemUuid) 
	: mItem(_item)
{
	mItem->setData(tag::data::uuid, itemUuid);
}

OptimaElement::OptimaElement() 
	: mItem(nullptr)
{

}

void OptimaElement::applyXml(const QDomNode & element, const QDomNode & elementPattern)
{
	OptimaXml::applyXml(element, elementPattern);

	applyCommonProperties();

}

void OptimaElement::applyCommonProperties()
{
	//Если нет гравического элемнта, унаследованного от этого класса, то ничего не загружаем
	if (mItem == nullptr)
	{
		return;
	}

	//Задаем порядок отображеняи на схеме
	qreal zOrder = getXmlValue(tag::order, 1.0);
	mItem->setZValue( ( zOrder >= 0.0 && zOrder <= Z_ORDER_DELIMITER ) ? zOrder / Z_ORDER_DELIMITER : 1.0 );

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

void OptimaElement::onMarkerMove(const OptimaBaseMarker* marker)
{
	throw std::logic_error("The method or operation is not implemented.");
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

void OptimaElement::debugShowXml()
{
	qDebug() << getXmlString(mNodePatternXml);

	qDebug() << getXmlString(mNodeXml);
}
