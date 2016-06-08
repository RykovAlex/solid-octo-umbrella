#pragma once
#include "optimaelement.h"
#include <QtXml/QDOMDocument>
#include "optimapoint.h"
#include "optimaconnectorarrow.h"
#include "optimapath.h"

class OptimaConnector : public QGraphicsPathItem, public OptimaElement
{
public:
	OptimaConnector(const QString &itemUuid);

	~OptimaConnector()
	{
	}
	
	///��������� �������� ��������� ����� �������� xml
	void apply();

	//������ ��������, ������ ��������� ����, ������ ����������� �� �����
	void draw();

	void getIntersection(const QList<QGraphicsItem*> &itemList);

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		QWidget *widget = 0);

private:
	OptimaPointVector mPoints;///< �������������� ����� ������, ��� ��������� �� �����

	OptimaConnectorArrow mBeginArrow;///< ������� � ������ ����������

	OptimaConnectorArrow mEndArrow;///< ������� � ����� ����������

	OptimaCross mCross;///< ����� �������� �� ��������� ����������� � ������� �����������
	
	qreal mRadiusCorner;///< ������ ���������� ����� ���������
	
	QPainterPath mPathArrow;
	
	OptimaPath mConnectorPath;

	bool mIsHighlight;

	QPen mPen;

	void intersected(const OptimaPath & connectorPath);
};


