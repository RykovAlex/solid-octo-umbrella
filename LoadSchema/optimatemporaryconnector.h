#pragma once
#include "optimaview.h"
#include "tag.h"
#include "optimapath.h"
#include "optimafigure.h"
#include "optimaconnectorpathfinder.h"

class OptimaPath;
class OptimaTemporaryConnector : public QGraphicsPathItem
{
public:
	enum Relationship
	{
		free_free	    = tag::element::free << 4 | tag::element::free
		,free_figure	= tag::element::free << 4 | tag::element::figure
		,free_connector	= tag::element::free << 4 | tag::element::connector
		,connector_free = tag::element::connector << 4 | tag::element::free
		,connector_connector = tag::element::connector << 4 | tag::element::connector
		,connector_figure = tag::element::connector << 4 | tag::element::figure
		,figure_free	= tag::element::figure << 4 | tag::element::free
		,figure_figure	= tag::element::figure << 4 | tag::element::figure
		,figure_connector = tag::element::figure << 4 | tag::element::connector
	} ;

	OptimaTemporaryConnector();

	OptimaTemporaryConnector(QGraphicsScene *scene, const  OptimaPointVector & points, const OptimaConnectorArrowVector & arrowVector = OptimaConnectorArrowVector() << OptimaConnectorArrow(connector_arrow_no, true)
		<< OptimaConnectorArrow(connector_arrow_filled, false), bool reversed = false);

	OptimaTemporaryConnector(const OptimaTemporaryConnector * tempConnector);

	void initialize();

	void createMarker(const QPointF & scenePos, const QLineF & makerVector);

	~OptimaTemporaryConnector();

	void setMarkerPen(QGraphicsRectItem *marker, QPointF scenePos);

	void setPoints(const OptimaPointVector & val);

	void onEndBorderMove(const QPointF & scenePos);
	
	void onBeginBorderMove(const QPointF & scenePos);
	
	void buildPath();

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

	OptimaPointVector points() const;

	void draw();

	virtual QPainterPath shape() const;

	inline QPointF first() const
	{
		return mPoints.first();
	}

	inline QPointF last() const
	{
		return mPoints.last();
	}

	inline Relationship getRelationship() const
	{
		return OptimaTemporaryConnector::Relationship(
			this->childItems().at(0)->data(tag::data::linkingElement).toInt() << 4 | 
			this->childItems().at(1)->data(tag::data::linkingElement).toInt() 
			); 
	}

	inline QPointF startPoint() const 
	{ 
		return mStartPoint; 
	}
	
	OptimaConnectorArrow beginArrow() const 
	{ 
		return mBeginArrow; 
	}

	OptimaConnectorArrow endArrow() const 
	{ 
		return mEndArrow; 
	}

	OptimaPointVector realPoints(const OptimaElement *startElement, const OptimaElement *endElement);

	bool isReversed() const { return mReversed; }
	
protected:

	static const int margin = 20;

	OptimaPointVector mPoints;///< действительные точки коннетора, хранимые в xml

	OptimaCross mCross;///< класс отвечает за отрисовку пересечений с другими коннеторами

	qreal mRadiusCorner;///< радиус скругления углов коннетора

	OptimaConnectorArrow mBeginArrow;///< стрелка в начале коннектора

	OptimaConnectorArrow mEndArrow;///< стрелка в конце коннектора

	OptimaPath mConnectorPath;///< путь для отрисовки коннетора по точкам mPoints  учетом закруглений и пересечений

private:

	QPainterPath mPathArrow;///< путь для отрисовки стрелок коннектора

	QPointF mStartPoint;///< точка от которой мы начинали строить коннектор

	bool mReversed;///< признак того, что точки коннектора надо обратить 

	inline bool isLinkedAt(const QPointF & scenePos )
	{
		return OptimaConnectorPathFinder::findLinkedItem(scene(), scenePos) != nullptr;
	}

	int getLinkingElementType(const QPointF & scenePos, int & linkingType );

};


