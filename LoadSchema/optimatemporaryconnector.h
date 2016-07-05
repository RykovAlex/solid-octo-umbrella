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

	OptimaTemporaryConnector(QGraphicsScene *scene, const  OptimaPointVector & points);

	OptimaTemporaryConnector(const OptimaTemporaryConnector * tempConnector);

	void initialize();

	void createMarker(const QPointF & scenePos);

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

	inline Relationship getRelationship()
	{
		return OptimaTemporaryConnector::Relationship(
			this->childItems().at(0)->data(tag::data::linkingElement).toInt() << 4 | 
			this->childItems().at(1)->data(tag::data::linkingElement).toInt() 
			); 
	}

	QPointF startPoint() const { return mStartPoint; }
	
protected:

	static const int margin = 20;

	OptimaPointVector mPoints;///< �������������� ����� ���������, �������� � xml

	OptimaCross mCross;///< ����� �������� �� ��������� ����������� � ������� �����������

	qreal mRadiusCorner;///< ������ ���������� ����� ���������

	OptimaConnectorArrow mBeginArrow;///< ������� � ������ ����������

	OptimaConnectorArrow mEndArrow;///< ������� � ����� ����������

	OptimaPath mConnectorPath;///< ���� ��� ��������� ��������� �� ������ mPoints  ������ ����������� � �����������

private:

	QPainterPath mPathArrow;///< ���� ��� ��������� ������� ����������

	QPointF mStartPoint;///< ����� �� ������� �� �������� ������� ���������

	inline QRectF getMarkerRect(QPointF scenePos) const 
	{
		return QRectF( scenePos - QPointF(3.0,3.0), scenePos + QPointF(3.0,3.0));
	}


	inline bool isLinkedAt(const QPointF & scenePos )
	{
		return OptimaConnectorPathFinder::findLinkedItem(scene(), scenePos) != nullptr;
	}

	int getLinkingElementType(const QPointF & scenePos, int & linkingType );
};


