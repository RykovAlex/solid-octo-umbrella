#pragma once
#include <QtXml/QDOMDocument>
#include "optimapoint.h"
#include "optimaconnectorarrow.h"
#include "OptimaCross.h"

class OptimaElement
{
public:
	OptimaElement(QGraphicsItem *_item, const QString &itemUuid);
	

	~OptimaElement()
	{
	}

	///�������� ���������� ������������ �������� �������� xml - ���������
	virtual void apply() = 0;
	virtual void draw()  = 0;

	///������� ����� xml
	void applyXml(const QDomNode & element);

protected:	
	///�������� �������� ������������� xml
	QString getXmlString(const QDomNode & element) const;

	///�������� �� xml - ��������� ����
	const QDomElement getXmlNode(const QString & name) const;

	qreal getXmlValue(const QString & name, const qreal defaultValue) const;
	void getXmlValue(const QString & name, OptimaPointVector &optimaPoints) const;
	void getXmlValue(const QString & name, OptimaPoint &optimaPoint) const;
	void getXmlValue(const QString & name, OptimaConnectorArrow &optimaConnectorArrow) const;
	void getXmlValue(const QString & name, OptimaCross &optimaCrossing) const;
	QColor getXmlValue(const QString & name, const QColor defaultColor) const;
	Qt::PenStyle getXmlValue(const QString & name, const Qt::PenStyle defaultStyle) const;
	QString getXmlValue(const QString & name, const QString &defaultString) const;
	const QFont getXmlValue(const QString & name, const QFont &defaultFont) const;

	void setXmlValue(const QString & name, const qreal value) const;

private:
	QGraphicsItem *mItem;///<��������� �� ����������� ������ - ������

	QDomNode mNodeXml;///<����� ����� ������� ������� xml - ��������� ��������

	///��������� ��������� �������� ������ xml - ��������� ����� �������� �������� � ������������ xml
	void updateXml(const QDomNode &element);

	///�������� ����� ���������� ��� ���� ����������� ��������� �������� xml - ���������
	void applyCommonProperties();
};


