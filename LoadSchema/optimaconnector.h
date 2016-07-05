#pragma once
#include "optimaelement.h"
#include <QtXml/QDOMDocument>
#include "optimapoint.h"
#include "optimaconnectorarrow.h"
#include "optimapath.h"
#include "optimaconnectormovemarker.h"
#include "optimatemporaryconnector.h"

class OptimaConnectorMoveMarker;

class OptimaConnector : public OptimaTemporaryConnector, public OptimaElement
{
public:
	enum { Type = UserType + tag::element::connector };
	OptimaConnector(const QString &itemUuid, OptimaView *view);

	OptimaConnector(const OptimaTemporaryConnector *tempConnector, OptimaView *view);

	void initialize();

	~OptimaConnector();
	
	///Применить изенения переданне через струтуру xml
	virtual void apply();

	///Рисует коннетор, точнее формирует путь, которй добавляется на схему
	virtual void draw(bool isProcessLoading = false);

	///Функция вызывается при перемещении маркера принадлежащего этому коннетору, перемещаемый пользователем маркер
	///передается в функцию
	virtual void onMarkerMove(const OptimaBaseMarker* marker);

	void getIntersection(const QList<QGraphicsItem*> &itemList, int start);

	void clearIntersection();

	virtual int type() const;

	virtual void setLinkedHighlight(bool enabled, const QPointF & scenePos = QPointF());

	virtual bool checkLinkedHighlight(const QPointF & scenePos);

	bool checkLinkedHighlight(const QPointF & scenePos, int &indexLinkedLine);

	QPainterPath getStrokeLinePath(const QLineF &line) const;

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

	inline OptimaLine linkedLine() const
	{
		Q_ASSERT(mIndexLinkedLine >= 0 && mIndexLinkedLine < (mPoints.size() - 1));

		return OptimaLine(getPathLine(mIndexLinkedLine), getUuidsLine(mIndexLinkedLine));
	}

protected:

	virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);


	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:

	bool mIsAngledСonnector;///< признак что это угловой коннетор

	void intersected(OptimaPath & connectorPath);
	
	void onLineMove(const OptimaConnectorMoveMarker* moveMarker);

// Reimplemented Protected Functions
	void mousePressEvent(QGraphicsSceneMouseEvent *event);

	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);

	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

	void createMarkers();

	void destroyMarkers();
	
	inline QLineF getPathLine(int indexLine) const
	{
		return mConnectorPath.lineAt(indexLine);
	}

	int mIndexLinkedLine;
	
	inline QVector<QString> getUuidsLine(int indexLine) const
	{
		Q_ASSERT(indexLine >= 0 && indexLine < (mPoints.size() - 1));

		QVector<QString> res;
		res << mPoints.at(mIndexLinkedLine).uuid() << mPoints.at(mIndexLinkedLine + 1).uuid();

		return res;
	}
	void onConnectorMove(QPointF deltaPoint);
};

inline bool isConnector(const QGraphicsItem* item)
{
	return (qgraphicsitem_cast<const OptimaConnector*>(item) != nullptr);
}


