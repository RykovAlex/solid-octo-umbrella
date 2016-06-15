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

	///Изменить переменные графического элемента согласно xml - описателю
	virtual void apply() = 0;
	
	///обеспечить отрисовку элемента н асхеме
	virtual void draw(bool isProcessLoading = false) = 0;

	///Принять новый xml
	void applyXml(const QDomNode & element);


	//События, реакцию на которые нужно прописать в каждом унаследованном классе
	///Мышь попала на объект
	virtual void onHoverEnter(QGraphicsSceneHoverEvent *event) = 0;

	///Мышь покинула объект
	virtual void onHoverLeave(QGraphicsSceneHoverEvent* hoverEvent) = 0;

	/// эта функция вызывается когда маркер привязанный к этому элементу перемещается пользователем
	virtual void onMarkerMove(const OptimaBaseMarker* marker) = 0;


protected:	
	OptimaView *mView;///< объект отвечающий за отображение схемы


	///Получить тектовое представление xml
	QString getXmlString(const QDomNode & element) const;

	///Получить из xml - описателя поле
	const QDomElement getXmlNode(const QString & name) const;

	///Получить из xml -описателя различные значения
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
	QGraphicsItem *mItem;///<Указатель на графический объект - предок

	QDomNode mNodeXml;///<Здесь будем хранить текущий xml - описатель элемента

	///Применить изменения согласно новому xml - описателю путем внесения изменний в существующий xml
	void updateXml(const QDomNode &element);

	///Изменить общие переменные для всех графических элементов согласно xml - описателю
	void applyCommonProperties();
};


