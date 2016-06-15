#pragma once
#include <QtXml/QDOMDocument>
#include "optimapoint.h"
#include "optimaconnectorarrow.h"
#include "OptimaCross.h"
#include "optimabasemarker.h"

class OptimaView;

class OptimaElement
{
public:
	OptimaElement();

	OptimaElement(QGraphicsItem *_item, const QString &itemUuid, OptimaView *view);
	

	~OptimaElement()
	{
	}

	///�������� ���������� ������������ �������� �������� xml - ���������
	virtual void apply() = 0;
	
	///���������� ��������� �������� � ������
	virtual void draw(bool isProcessLoading = false) = 0;

	///������� ����� xml
	void applyXml(const QDomNode & element);


	//�������, ������� �� ������� ����� ��������� � ������ �������������� ������
	///���� ������ �� ������
	virtual void onHoverEnter(QGraphicsSceneHoverEvent *event) = 0;

	///���� �������� ������
	virtual void onHoverLeave(QGraphicsSceneHoverEvent* hoverEvent) = 0;

	/// ��� ������� ���������� ����� ������ ����������� � ����� �������� ������������ �������������
	virtual void onMarkerMove(const OptimaBaseMarker* marker) = 0;


protected:	
	OptimaView *mView;///< ������ ���������� �� ����������� �����


	///�������� �������� ������������� xml
	QString getXmlString(const QDomNode & element) const;

	///�������� �� xml - ��������� ����
	const QDomElement getXmlNode(const QString & name) const;

	///�������� �� xml -��������� ��������� ��������
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


