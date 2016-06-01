#pragma once
#include <QtXml/QDOMDocument>
#include "optimapoint.h"
#include "optimaconnectorarrow.h"

class OptimaElement
{
public:
	OptimaElement(QGraphicsItem *_item, const QString &itemUuid);
	

	~OptimaElement()
	{
	}

	virtual void apply(const QDomNode &) = 0;
	virtual void draw()  = 0;

protected:	
	typedef QVector<OptimaPoint> OptimaPointVector;

	///�������� �������� ������������� xml
	QString getXmlString(const QDomNode & element) const;

	///������� ����� xml
	void applyXml(const QDomNode & element);

	///�������� �� xml - ��������� ����
	const QDomElement getXmlNode(const QString & name) const;
	
	qreal getXmlValue(const QString & name, const qreal defaultValue) const;
	void getXmlValue(const QString & name, QVector<OptimaPoint> &optimaPoints) const;
	void getXmlValue(const QString & name, OptimaPoint &optimaPoint) const;
	void getXmlValue(const QString & name, OptimaConnectorArrow &optimaConnectorArrow) const;
	void setXmlValue(const QString & name, const qreal value) const;
private:
	QGraphicsItem *mItem;///<��������� �� ����������� ������ - ������

	QDomNode mNodeXml;///<����� ����� ������� ������� xml - ��������� ��������

	///��������� ��������� �������� ������ xml - ��������� ����� �������� �������� � ������������ xml
	void updateXml(const QDomNode &element);

	void applyCommonProperties();
};


