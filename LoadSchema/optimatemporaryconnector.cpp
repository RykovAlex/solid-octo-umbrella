#include "stdafx.h"
#include "optimatemporaryconnector.h"



OptimaTemporaryConnector::OptimaTemporaryConnector(QGraphicsScene *scene, const OptimaPointVector & points
	, const OptimaConnectorArrowVector & arrowVector /*= OptimaConnectorArrowVector() << OptimaConnectorArrow(connector_arrow_no, true)	<< OptimaConnectorArrow(connector_arrow_filled, false)*/
	, bool reversed
	) 
	: mBeginArrow(arrowVector.at(0))
	, mEndArrow(arrowVector.at(1))
	, mPoints(points)
	, mReversed(reversed)
{
	initialize();
	scene->addItem(this);
	
	if (reversed)
	{
		createMarker(points.last(), 1);

		createMarker(points.first(), 0);

		mStartPoint = points.last();
	}
	else
	{
		createMarker(points.first(), 0);

		createMarker(points.last(), 1);

		mStartPoint = points.first();
	}
}

OptimaTemporaryConnector::OptimaTemporaryConnector() : mBeginArrow(connector_arrow_no, true)
	, mEndArrow(connector_arrow_filled, false)
	, mStartPoint(QPointF(0.0,0.0))
	, mReversed(false)
{
	initialize();
}

OptimaTemporaryConnector::OptimaTemporaryConnector(const OptimaTemporaryConnector * tempConnector)
	: mBeginArrow(connector_arrow_no, true)
	, mEndArrow(connector_arrow_filled, false)
	, mPoints(tempConnector->points())
	, mStartPoint(tempConnector->startPoint())
	, mReversed(false)
{
	initialize();
}

void OptimaTemporaryConnector::initialize()
{
	mRadiusCorner = 0;
	setData(tag::data::linkable, false);

	setAcceptedMouseButtons(Qt::NoButton);
	setAcceptHoverEvents(false);
	//setFlag(ItemIsSelectable);
	//setFlag(ItemIsMovable);

	setPen(QPen(Qt::black, 1, Qt::DashDotLine));

}

void OptimaTemporaryConnector::createMarker(const QPointF & scenePos, int borderIndex)
{
	QGraphicsRectItem *marker = new QGraphicsRectItem(OptimaConnectorPathFinder::getMarkerRect(QPointF(0.0,0.0)), this);
	marker->setData(tag::data::borderIndex, borderIndex);
	marker->setPos(scenePos);
	setMarkerPen(marker, scenePos);

}

OptimaTemporaryConnector::~OptimaTemporaryConnector()
{

}

void OptimaTemporaryConnector::setMarkerPen(QGraphicsRectItem * marker, QPointF scenePos)
{
	int linkingParameter = -1;
	int linkingElementType = getLinkingElementType(scenePos, linkingParameter);
	
	marker->setData(tag::data::linkingElement, linkingElementType);
	marker->setData(tag::data::linkingParameter, linkingParameter);

	QPen markerPen( linkingElementType == tag::element::free ? tag::markerFreeColor : tag::markerLinkColor, 1, Qt::SolidLine);
	marker->setPen(markerPen);
}

void OptimaTemporaryConnector::setPoints(const OptimaPointVector & val)
{
	mPoints = val; 	

	buildPath(); 

	// обозначим пространство, в котором будем рисовать коннетор
	draw();
}

void OptimaTemporaryConnector::onEndBorderMove(const QPointF & scenePos, QPointF & beginPoint, QPointF & endPoint)
{
	Q_ASSERT(this->childItems().count() >=  2);

	QGraphicsRectItem * borderEnd = dynamic_cast<QGraphicsRectItem *>(this->childItems().at(1));
	setMarkerPen(borderEnd, scenePos);

	borderEnd->setPos( scenePos );

	//если передвигаем начало коннеткора, а не конец
	if (isReversed())
	{
		beginPoint = scenePos;
		endPoint = last();
	}
	else
	{
		beginPoint = first();
		endPoint = scenePos;
	}
}

void OptimaTemporaryConnector::onBeginBorderMove(const QPointF & scenePos)
{
	Q_ASSERT(this->childItems().count() >=  2);

	QGraphicsRectItem * borderBegin = dynamic_cast<QGraphicsRectItem *>(this->childItems().at(0));
	setMarkerPen(borderBegin, scenePos);
	borderBegin->setPos( scenePos );
}

void OptimaTemporaryConnector::buildPath()
{
	Q_ASSERT(mPoints.size() >= 2);
	const OptimaCross & cross = mCross;

	//Здесь будут изображения стрелок начала и конца коннетора
	QPainterPath pathArrow;

	// рисуем стрелку начала, и соотвественно меняем начальную точку отрисовки коннектора
	QPointF startPoint = beginArrow().getPath(pathArrow, mPoints.at( 0 ), mPoints.at( 1 ));

	mConnectorPath = OptimaPath(startPoint, cross);

	for ( int i = 1; i < mPoints.size( ) - 1; ++i )
	{
		mConnectorPath.lineTo(mPoints.at( i ), QLineF(mPoints.at( i + 1), mPoints.at( i )), mRadiusCorner);
	}

	// рисуем конец
	const QPointF endPoint = mEndArrow.getPath( pathArrow, mPoints.at( mPoints.size() - 1 ), mPoints.at( mPoints.size() - 2 ));

	mConnectorPath.lineTo(endPoint);

	// сохраним для отрисовки в paint()
	mPathArrow = pathArrow;
}

void OptimaTemporaryConnector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
	//рисуем по своему, чтобы заблокировать стандартную отрисовку выделения объекта
	painter->setPen(pen());
	painter->drawPath(path());

	painter->setBrush(this->pen().color());
	painter->drawPath(mPathArrow);
}

OptimaPointVector OptimaTemporaryConnector::points() const
{
	return mPoints;
}

void OptimaTemporaryConnector::draw()
{
	setPos(0.0, 0.0);
	setPath( mConnectorPath.toPath() );
}

QPainterPath OptimaTemporaryConnector::shape() const
{
	QPainterPath path(this->path());
	QPainterPathStroker pathStrocke;
	pathStrocke.setWidth(margin);

	return pathStrocke.createStroke(path);
}

OptimaPointVector OptimaTemporaryConnector::realPoints(const OptimaElement *startElement, const OptimaElement *endElement)
{
	OptimaPointVector points(mPoints);
	QGraphicsRectItem * borderEnd = dynamic_cast<QGraphicsRectItem *>(this->childItems().at(1));

	//если передвигаем начало коннеткора, а не конец
	if ( isReversed() )
	{
		if (endElement != nullptr)
		{
			points.first() = endElement->getIntersectPoint(QLineF(mPoints.at(0), mPoints.at(1)));
		}
		if ( startElement != nullptr)
		{
			points.last() = startElement->getIntersectPoint(QLineF(mPoints.at(mPoints.size()-1), mPoints.at(mPoints.size()-2)));
		}
	}
	else
	{
		if (startElement != nullptr)
		{
			points.first() = startElement->getIntersectPoint(QLineF(mPoints.at(0), mPoints.at(1)));
		}
		if (endElement != nullptr)
		{
			points.last() = endElement->getIntersectPoint(QLineF(mPoints.at(mPoints.size()-1), mPoints.at(mPoints.size()-2)));
		}

	}
	
	return points;
}

int OptimaTemporaryConnector::getLinkingElementType(const QPointF & scenePos, int & linkingType)
{
	QGraphicsItem * item = OptimaConnectorPathFinder::findLinkedItem(scene(), scenePos);

	if (item == nullptr)
	{
		return tag::element::free;
	}

	linkingType = item->data(tag::data::linkingParameter).toInt();
	
	return item->type() - UserType;
}

bool OptimaTemporaryConnector::isReversed() const
{
	return mReversed;
}
