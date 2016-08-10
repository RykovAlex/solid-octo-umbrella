#pragma once
#include <QtXml/QDOMDocument>
#include "optimapoint.h"

class OptimaConnectorArrow;
class OptimaCross;
class OptimaXml
{
public:
	OptimaXml();

	~OptimaXml();

	///Изменить переменные графического элемента согласно xml - описателю
	virtual void apply() = 0;

	static QDomElement nodeXml(QDomNode node, const QString & name);

	static void setXmlValue(QDomNode & node, const QString & name, const QString &value);
	
	static void setXmlValue(QDomNode & node, const OptimaPointVector &points, const QString &ownerId);
	static void setXmlValue(QDomNode & node, const QString & name, const qreal value);
	void setXmlValue(const QString & name, const qreal value);
	void setXmlValue(const QString & name, const QString &value);

	void setXmlValue(const OptimaPointVector &points, const QString &ownerId);
	void applyXml(const QDomNode & element, const QDomNode & elementPattern);

	///Получить из xml -описателя различные значения
	void getXmlValue(const QString & name, OptimaPointVector &optimaPoints) const;
	void getXmlValue(const QString & name, OptimaPoint &optimaPoint) const;
	void getXmlValue(const QString & name, OptimaConnectorArrow &optimaConnectorArrow) const;
	void getXmlValue(const QString & name, OptimaCross &optimaCrossing) const;
	Qt::PenStyle getXmlValue(const QString & name, const Qt::PenStyle defaultStyle) const;
	QString getXmlValue(const QString & name, const QString &defaultString) const;
	const QFont getXmlValue(const QString & name, const QFont &defaultFont) const;
	qreal getXmlValue(const QString & name, const qreal defaultValue) const;
	QColor getXmlValue(const QString & name, const QColor defaultColor) const;

	QString getXmlString();
protected:
	///Получить тектовое представление xml
	static QString getXmlString(const QDomNode & element);

	QDomNode mNodeXml;///<Здесь будем хранить текущий xml - описатель элемента
	
	QDomNode mNodePatternXml;///<Здесь будем хранить текущий xml - описатель шаблона

	void updateXml(const QDomNode &element);
	
private:

	///Получить из xml - описателя поле
	const QDomElement nodeXml(const QString & name) const;

};


