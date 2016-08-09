#include "stdafx.h"
#include "optimatemporaryconnector.h"
#include "optimaconnector.h"

OptimaTemporaryConnector::OptimaTemporaryConnector(OptimaConnector * rebuildingConnector ) 
	: mBeginArrow(rebuildingConnector->beginArrow())
	, mEndArrow(rebuildingConnector->endArrow())
	, mPoints(rebuildingConnector->points())
{
	initialize();
	rebuildingConnector->scene()->addItem(this);
}

OptimaTemporaryConnector::OptimaTemporaryConnector(QGraphicsScene *scene, const OptimaPointVector & points
	, const OptimaConnectorArrowVector & arrowVector /*= OptimaConnectorArrowVector() << OptimaConnectorArrow(connector_arrow_no, true)	<< OptimaConnectorArrow(connector_arrow_filled, false)*/
	) 
	: mBeginArrow(arrowVector.at(0))
	, mEndArrow(arrowVector.at(1))
	, mPoints(points)
{
	initialize();
	scene->addItem(this);
}

OptimaTemporaryConnector::OptimaTemporaryConnector() : mBeginArrow(connector_arrow_no, true)
	, mEndArrow(connector_arrow_filled, false)
{
	initialize();
}

OptimaTemporaryConnector::OptimaTemporaryConnector(const OptimaTemporaryConnector * tempConnector)
	: mBeginArrow(connector_arrow_no, true)
	, mEndArrow(connector_arrow_filled, false)
	, mPoints(tempConnector->points())
{
	initialize();
}

void OptimaTemporaryConnector::initialize()
{
	mRadiusCorner = 0;
	setData(tag::data::linkable, false);

	setAcceptedMouseButtons(Qt::NoButton);
	setAcceptHoverEvents(false);

	setPen(QPen(Qt::black, 1, Qt::DashDotLine));
}

OptimaTemporaryConnector::~OptimaTemporaryConnector()
{
}

void OptimaTemporaryConnector::setPoints(const OptimaPointVector & val)
{
	mPoints = val; 	

	buildPath(); 

	// обозначим пространство, в котором будем рисовать коннетор
	draw();
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
	mConnectorCleanPath = OptimaPath(mPoints.first(), OptimaCross());

	for ( int i = 1; i < mPoints.size( ) - 1; ++i )
	{
		mConnectorPath.lineTo(mPoints.at( i ), QLineF(mPoints.at( i + 1), mPoints.at( i )), mRadiusCorner);
		mConnectorCleanPath.lineTo(mPoints.at( i ), QLineF(mPoints.at( i + 1), mPoints.at( i )), mRadiusCorner);
	}

	// рисуем конец
	const QPointF endPoint = mEndArrow.getPath( pathArrow, mPoints.at( mPoints.size() - 1 ), mPoints.at( mPoints.size() - 2 ));

	mConnectorPath.lineTo(endPoint);
	mConnectorCleanPath.lineTo(mPoints.last());

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
	//QPainterPath path(this->path());
	QPainterPath path(mConnectorCleanPath.toPath());

	QPainterPathStroker pathStrocke;
	pathStrocke.setWidth(margin);

	return pathStrocke.createStroke(path);
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