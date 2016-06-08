#include "stdafx.h"
#include "optimaconnector.h"
#include "tag.h"

OptimaConnector::OptimaConnector(const QString &itemUuid) 
	:OptimaElement(this, itemUuid)
	,mBeginArrow(connector_arrow_no, true)
	,mEndArrow(connector_arrow_no, false)
	,mIsHighlight(false)
{
}

void OptimaConnector::apply()
{
	//1. Заполним рабочие переменные
	//Эти переменные выделены из XML потому что испльзуются при отрисовке
	//Значение неизменно. не требует изменения XML при сохранении элемента
	getXmlValue(tag::shape_begin, mBeginArrow);
	mBeginArrow.setSize(getXmlValue(tag::size_shape_begin, 10.0));

	getXmlValue(tag::shape_end, mEndArrow);
	mEndArrow.setSize(getXmlValue(tag::size_shape_end, 10.0));

	getXmlValue(tag::cross_type, mCross);
	
	mRadiusCorner = getXmlValue(tag::radius_corner, 0.0);

	//Сохраняем текущий карандаш, так как надо отрабатывать выделение коннеторов при подводе к ним мышки
	//Начальные настройки карандаша устанавливаюся в applyXml
	mPen = pen();	
	
	//Значения этих переменных вынесены в переменные, потому что они интерактивно изменяются пользователем,
	//Требуется изменить XML перед сохранением, длясохранения действий пользователя
	//Непосредственное испрользование из XML требует затрат процессора на постоянное извлечение и перезапись,
	//нерационально
	getXmlValue(tag::structure_dot, mPoints );
}

void OptimaConnector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
	if (mIsHighlight)
	{
		QPen localPen(mPen);
		localPen.setWidth(mPen.width() + 1);
		setPen(localPen);
	}
	else
	{
		setPen(mPen);
	}
	QGraphicsPathItem::paint(painter, option, widget);

	painter->setBrush(this->pen().color());
	painter->drawPath(mPathArrow);
}

void OptimaConnector::draw()
{	
	Q_ASSERT(mPoints.size() >= 2);

	//Здесь будут изображения стрелок начала и конца коннетора
	QPainterPath pathArrow;
	
	// рисуем стрелку начала, и соотвественно меняем начальную точку отрисовки коннектора
	QPointF startPoint = mBeginArrow.getPath(pathArrow, mPoints.at( 0 ), mPoints.at( 1 ));

	mConnectorPath = OptimaPath(startPoint);

	for ( int i = 1; i < mPoints.size( ) - 1; ++i )
	{
		mConnectorPath.lineTo(mPoints.at( i ), QLineF(mPoints.at( i + 1), mPoints.at( i )), mRadiusCorner);
	}

	// рисуем конец
	const QPointF endPoint = mEndArrow.getPath( pathArrow, mPoints.at( mPoints.size() - 1 ), mPoints.at( mPoints.size() - 2 ));
	
	mConnectorPath.lineTo(endPoint);
	
	// сохраним для отрисовки в paint()
	mPathArrow = pathArrow;

	//for ( int i = 1; i < points.size(); ++i )
	//{
	//	// точки пересечения с текущим отрезком храняться от начала отрезка
	//	const QLineF originalLine(points.at( i - 1 ), points.at( i ));
	//	mCross.draw( pathLine, originalLine,  mPoints.at(i - 1).getCrossingWithConnetorLengths(), lengthFromBegin );

	//}
	
	setPath( mConnectorPath.toPath() );
	
	//if ( is_selected( ) )
	//{
	//	redraw_coners( redraw_borders );
	//}
}

void OptimaConnector::intersected(const OptimaPath & connectorPath)
{
	mConnectorPath.intersected(connectorPath);
}

void OptimaConnector::getIntersection(const QList<QGraphicsItem*> &itemList)
{
	for (QList<QGraphicsItem*>::const_iterator i = itemList.constBegin(); i != itemList.constEnd(); ++i )
	{
		OptimaConnector* item = dynamic_cast<OptimaConnector*>(*i);
		if (item == nullptr)
		{
			continue;
		}
		if (item == this)
		{
			continue;
		}
		if (zValue() > item->zValue())
		{
			intersected(item->mConnectorPath);
		}
		else
		{
			item->intersected(mConnectorPath);
		}		
	}
}
