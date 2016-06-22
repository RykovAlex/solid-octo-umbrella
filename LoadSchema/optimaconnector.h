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

	~OptimaConnector();
	
	///��������� �������� ��������� ����� �������� xml
	virtual void apply();

	///������ ��������, ������ ��������� ����, ������ ����������� �� �����
	virtual void draw(bool isProcessLoading = false);

	///������� ���������� ��� ����������� ������� �������������� ����� ���������, ������������ ������������� ������
	///���������� � �������
	virtual void onMarkerMove(const OptimaBaseMarker* marker);

	void buildPath(const OptimaCross & cross);

	void getIntersection(const QList<QGraphicsItem*> &itemList, int start);

	void clearIntersection();

	virtual QPainterPath shape() const;

protected:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		QWidget *widget = 0);

	virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);



private:
	OptimaPointVector mPoints;///< �������������� ����� ���������, �������� � xml

	OptimaConnectorArrow mBeginArrow;///< ������� � ������ ����������

	OptimaConnectorArrow mEndArrow;///< ������� � ����� ����������

	OptimaCross mCross;///< ����� �������� �� ��������� ����������� � ������� �����������
	
	qreal mRadiusCorner;///< ������ ���������� ����� ���������
	
	bool mIsAngled�onnector;///< ������� ��� ��� ������� ��������

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


