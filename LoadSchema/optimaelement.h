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
	///Получить тектовоу представление xml
	QString getXmlString(const QDomNode & element) const;

	///Принять новый xml
	void applyXml(const QDomNode & element);

	///Получить из xml - описателя поле
	const QDomElement getXmlNode(const QString & name) const;
private:
	QGraphicsItem *mItem;///<Указатель на графический объект - предок

	QDomNode mNodeXml;///<Здесь будем хранить текущий xml - описатель элемента

	///Применить изменения согласно новому xml - описателю путем внесения изменний в существующий xml
	void updateXml(const QDomNode &element);

};


