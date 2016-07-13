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


	/// ��� ������� ���������� ����� ������ ����������� � ����� �������� ������������ �������������
	virtual void onMarkerMove(const OptimaBaseMarker* marker) = 0;

	inline const OptimaView * view()
	{
		return mView;
	}

	virtual void setLinkedHighlight(bool enabled, const QPointF & scenePos = QPointF()) = 0;

	virtual bool checkLinkedHighlight(const QPointF & scenePos) = 0;

	virtual QPointF getIntersectPoint(const QLineF line) const = 0;

	QString uuid() const;

	static void setXmlZOrder(QDomNode & node, qreal sceneZOrder);
protected:	
	OptimaView *mView;///< ������ ���������� �� ����������� �����

	///�������� �������� ������������� xml
	QString getXmlString(const QDomNode & element) const;

	///�������� �� xml - ��������� ����
	const QDomElement getXmlNode(const QString & name) const;
	
	static QDomElement getXmlNode(QDomNode node, const QString & name);

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

	void setXmlValue(const QString & name, const qreal value);

	void setXmlValue(const QString & name, const QString &value);

	static void setXmlValue(QDomNode & node, const QString & name, const qreal value);

	static void setXmlValue(QDomNode & node, const OptimaPointVector &points,const QString &ownerId);
	
	static void setXmlValue(QDomNode & node, const QString & name, const QString &value);

private:
	QGraphicsItem *mItem;///<��������� �� ����������� ������ - ������

	QDomNode mNodeXml;///<����� ����� ������� ������� xml - ��������� ��������

	///��������� ��������� �������� ������ xml - ��������� ����� �������� �������� � ������������ xml
	void updateXml(const QDomNode &element);

	///�������� ����� ���������� ��� ���� ����������� ��������� �������� xml - ���������
	void applyCommonProperties();
};


