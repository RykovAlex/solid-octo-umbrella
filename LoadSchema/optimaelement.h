#pragma once
#include <QtXml/QDOMDocument>

class OptimaElement
{
private:
	QGraphicsItem *item;///<Указатель на графический объект - предок
	QDomNode nodeXml;///<Здесь будем хранить текущий xml - описатель элемента

	///Применить изменения согласно новому xml - описателю путем внесения изменний в существующий xml
	void updateXml(const QDomNode &element);

protected:	
	///Получить тектовоу представление xml
	QString getXml(const QDomNode & element);

	QPointF convert( const QString & text, int & radius);
	///Принять новый xml
	void applyXml(const QDomNode & element);

public:
	OptimaElement(QGraphicsItem *_item);
	

	~OptimaElement()
	{
	}

protected:

};


