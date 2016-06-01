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

private:
	OptimaPointVector mPoints;///< �������������� ����� ������, ��� ��������� �� �����

	OptimaConnectorArrow mBeginArrow;///<������� � ������ ����������

	OptimaConnectorArrow mEndArrow;///<������� � ����� ����������

	void draw();

	const QPointF & drawArrow(QPainterPath pp_arrows, UnknownType param2, UnknownType param3, OptimaConnectorArrow beginArrow, bool param5);
};


