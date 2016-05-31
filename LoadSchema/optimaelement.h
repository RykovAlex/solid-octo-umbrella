#pragma once
#include <QtXml/QDOMDocument>
#include "optimapoint.h"

class OptimaElement
{
public:
	OptimaElement(QGraphicsItem *_item);
	

	~OptimaElement()
	{
	}

protected:	
	///�������� �������� ������������� xml
	QString getXmlString(const QDomNode & element) const;

	///������� ����� xml
	void applyXml(const QDomNode & element);

	///�������� �� xml - ��������� ����
	const QDomElement getXmlNode(const QString & name) const;
private:
	QGraphicsItem *mItem;///<��������� �� ����������� ������ - ������

	QDomNode mNodeXml;///<����� ����� ������� ������� xml - ��������� ��������

	///��������� ��������� �������� ������ xml - ��������� ����� �������� �������� � ������������ xml
	void updateXml(const QDomNode &element);

};


