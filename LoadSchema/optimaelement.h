#pragma once
#include <QtXml/QDOMDocument>

class OptimaElement
{
private:
	QGraphicsItem *item;///<��������� �� ����������� ������ - ������
	QDomNode nodeXml;///<����� ����� ������� ������� xml - ��������� ��������

	///��������� ��������� �������� ������ xml - ��������� ����� �������� �������� � ������������ xml
	void updateXml(const QDomNode &element);

protected:	
	///�������� �������� ������������� xml
	QString getXml(const QDomNode & element);

	QPointF convert( const QString & text, int & radius);
	///������� ����� xml
	void applyXml(const QDomNode & element);

public:
	OptimaElement(QGraphicsItem *_item);
	

	~OptimaElement()
	{
	}

protected:

};


