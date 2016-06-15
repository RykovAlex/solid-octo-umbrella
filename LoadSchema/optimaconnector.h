#pragma once
#include "optimaelement.h"
#include <QtXml/QDOMDocument>
#include "optimapoint.h"
#include "optimaconnectorarrow.h"
#include "optimapath.h"
#include "optimaconnectormovemarker.h"

class OptimaConnector : public QGraphicsPathItem, public OptimaElement
{
public:
	OptimaConnector(const QString &itemUuid, OptimaView *view);

	~OptimaConnector()
	{
	}
	
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


	virtual void onHoverEnter(QGraphicsSceneHoverEvent *event);

	virtual void onHoverLeave(QGraphicsSceneHoverEvent* hoverEvent);

	bool isIntersected( const QRectF & rect ) const;



protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		QWidget *widget = 0);



private:
	OptimaPointVector mPoints;///< �������������� ����� ������, ��� ��������� �� �����

	OptimaConnectorArrow mBeginArrow;///< ������� � ������ ����������

	OptimaConnectorArrow mEndArrow;///< ������� � ����� ����������

	OptimaCross mCross;///< ����� �������� �� ��������� ����������� � ������� �����������
	
	qreal mRadiusCorner;///< ������ ���������� ����� ���������
	
	bool mIsAngled�onnector;///< ������� ��� ��� ������� ��������

	QPainterPath mPathArrow;
	
	OptimaPath mConnectorPath;

	bool mIsHighlight;

	QPen mPen;

	void intersected(OptimaPath & connectorPath);
	
	void moveLineEvent(const OptimaConnectorMoveMarker* moveMarker);

// Reimplemented Protected Functions
	void mousePressEvent(QGraphicsSceneMouseEvent *event);


};


