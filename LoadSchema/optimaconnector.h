#pragma once
#include "optimaelement.h"
#include <QtXml/QDOMDocument>
#include "optimapoint.h"
#include "optimaconnectorarrow.h"
#include "optimapath.h"
#include "optimatemporaryconnector.h"
#include "optimaconnectorlinemarker.h"
#include "optimaconnectormovemarker.h"

class OptimaConnectorMoveMarker;
class OptimaConnectorLineMarker;

class OptimaConnector : public OptimaTemporaryConnector, public OptimaElement
{
public:
	enum { Type = UserType + tag::element::connector };

	OptimaConnector(const QString &itemUuid);

	void initialize();

	~OptimaConnector();
	
	///��������� �������� ��������� ����� �������� xml
	virtual void apply();

	///������ ��������, ������ ��������� ����, ������ ����������� �� �����
	virtual void draw(bool isProcessLoading = false);

	///������� ���������� ��� ����������� ������� �������������� ����� ���������, ������������ ������������� ������
	///���������� � �������
	virtual void onMarkerMove(const OptimaBaseMarker* marker);

	void getIntersection(const QList<QGraphicsItem*> &itemList, int start);

	void clearIntersection();

	virtual int type() const;

	virtual void setLinkedHighlight(bool enabled, const QPointF & scenePos = QPointF());

	virtual bool checkLinkedHighlight(const QPointF & scenePos);

	bool checkLinkedHighlight(const QPointF & scenePos, int &indexLinkedLine) const;

	bool getLinkedLine(const QPointF & scenePos, OptimaLine &linkedLine) const;
	QPainterPath getStrokeLinePath(const QLineF &line) const;

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

	inline OptimaLine linkedLine() const
	{
		Q_ASSERT(mIndexLinkedLine >= 0 && mIndexLinkedLine < (mPoints.size() - 1));

		return OptimaLine(getPathLine(mIndexLinkedLine), getUuidsLine(mIndexLinkedLine));
	}

	bool isRebuild() const { return mRebuild; }
	
	void setRebuild(bool val, int changedIndex = -1);

	void setPoints(const OptimaPointVector & val, bool isUndo = false);

	void rebuildMarkers();

	virtual QPointF getIntersectPoint(const QLineF line) const;

	void setLinked(OptimaElement * linkedBeginElement, OptimaElement * linkedEndElement);

	OptimaElement *getBorderLinking(const QString &nameId);

	OptimaScene *scene();

	inline int getChangeIndex() const {return mChangeIndex;}

protected:

	virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);


	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:

	bool mIsAngled�onnector;///< ������� ��� ��� ������� ��������

	int mChangeIndex;///<�� ����� ��������� ��������� ����������, ����� ����� ������� ������ ����� ��� �����, ������� ����������

	void intersected(OptimaPath & connectorPath);
	
	void onLineMove(const OptimaConnectorLineMarker * moveMarker);

	void acceptNewPoint(OptimaPoint p, int indexLine, int insertPosition);

// Reimplemented Protected Functions
	void mousePressEvent(QGraphicsSceneMouseEvent *event);

	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);

	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

	void createMarkers();

	bool destroyMarkers();
	
	inline QLineF getPathLine(int indexLine) const
	{
		return mConnectorPath.lineAt(indexLine);
	}

	int mIndexLinkedLine;///< ������ ������� � �������� ���������������� ������ ���������

	bool mRebuild;///< �������, ��� ���� ��������� ������������ �����������
	
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
	
	qreal mNonSelectedZOrder;

	void hideMarkers(const OptimaConnectorLineMarker* lineMarker, bool hide);
	

};

inline bool isConnector(const QGraphicsItem* item)
{
	return (qgraphicsitem_cast<const OptimaConnector*>(item) != nullptr);
}


