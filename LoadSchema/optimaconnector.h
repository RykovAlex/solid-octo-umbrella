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

	bool isRebuild() const { return mRebuild; }
	
	void setRebuild(bool val, bool reversed);

	void setPoints(const OptimaPointVector & val);

	void rebuildMarkers();

	virtual QPointF getIntersectPoint(const QLineF line) const;

	void setLinked(OptimaElement * linkedBeginElement, OptimaElement * linkedEndElement);
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

	int mIndexLinkedLine;///< индекс отрезка к которому прилинковывается другой коннектор

	bool mRebuild;///< признак, что этот коннектор подвергается перестройке
	
	inline QVector<QString> getUuidsLine(int indexLine) const
	{
		Q_ASSERT(indexLine >= 0 && indexLine < (mPoints.size() - 1));

		QVector<QString> res;
		res << mPoints.at(mIndexLinkedLine).uuid() << mPoints.at(mIndexLinkedLine + 1).uuid();

		return res;
	}

	void onConnectorMove(QPointF deltaPoint);

	void setBorderId(const QString &nameId, const OptimaElement *element);

	bool checkBorderLinking(const QString &nameId);
};

inline bool isConnector(const QGraphicsItem* item)
{
	return (qgraphicsitem_cast<const OptimaConnector*>(item) != nullptr);
}


