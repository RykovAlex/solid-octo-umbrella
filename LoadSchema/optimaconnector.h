#pragma once
#include "optimaelement.h"
#include <QtXml/QDOMDocument>
#include "optimapoint.h"
#include "optimaconnectorarrow.h"
#include "optimapath.h"
#include "optimaconnectormovemarker.h"

class OptimaConnectorMoveMarker;

class OptimaConnector : public QGraphicsPathItem, public OptimaElement
{
public:
	OptimaConnector(const QString &itemUuid, OptimaView *view);

	OptimaConnector(OptimaView *view, const OptimaPointVector & points);

	~OptimaConnector();
	
	///Применить изенения переданне через струтуру xml
	virtual void apply();

	///Рисует коннетор, точнее формирует путь, которй добавляется на схему
	virtual void draw(bool isProcessLoading = false);

	///Функция вызывается при перемещении маркера принадлежащего этому коннетору, перемещаемый пользователем маркер
	///передается в функцию
	virtual void onMarkerMove(const OptimaBaseMarker* marker);

	void buildPath();

	void getIntersection(const QList<QGraphicsItem*> &itemList, int start);

	void clearIntersection();

	OptimaPointVector points() const { return mPoints; }

	void setPoints(const OptimaPointVector & val);

	OptimaPoint first() const { return mPoints.first(); }
	
	OptimaPoint last() const { return mPoints.last(); }

protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		QWidget *widget = 0);

	virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	virtual QPainterPath shape() const;

private:
	OptimaPointVector mPoints;///< действительные точки коннетора, хранимые в xml

	OptimaConnectorArrow mBeginArrow;///< стрелка в начале коннектора

	OptimaConnectorArrow mEndArrow;///< стрелка в конце коннектора

	OptimaCross mCross;///< класс отвечает за отрисовку пересечений с другими коннеторами
	
	qreal mRadiusCorner;///< радиус скругления углов коннетора
	
	bool mIsAngledСonnector;///< признак что это угловой коннетор

	QPainterPath mPathArrow;
	
	OptimaPath mConnectorPath;

	void intersected(OptimaPath & connectorPath);
	
	void onLineMove(const OptimaConnectorMoveMarker* moveMarker);

// Reimplemented Protected Functions
	void mousePressEvent(QGraphicsSceneMouseEvent *event);

	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);

	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);


	void createMarkers();
	void destroyMarkers();
};


