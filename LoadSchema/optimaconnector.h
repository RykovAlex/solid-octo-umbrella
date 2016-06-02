#pragma once
#include "optimaelement.h"
#include <QtXml/QDOMDocument>
#include "optimapoint.h"
#include "optimaconnectorarrow.h"

class OptimaConnector : public QGraphicsPathItem, public OptimaElement
{
public:
	OptimaConnector(const QString &itemUuid);

	~OptimaConnector()
	{
	}
	
	///��������� �������� ��������� ����� �������� xml
	void apply(const QDomNode & connector);

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
	//������ ��������, ������ ��������� ����, ������ ����������� �� �����
	void draw();
	
	qreal getCircleRadius(const int indexCorner) const;
	
	void drawCorner( QPainterPath &path, const QLineF originalLine, const int indexCorner ) const;

	bool mIsHighlight;
	QPen mPen;
};


