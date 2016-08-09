#pragma once
#include "optimapoint.h"
#include "optimaconnectorarrow.h"
#include "OptimaCross.h"
#include "optimabasemarker.h"
#include "optimaxml.h"

class OptimaScene;
class OptimaElement : public OptimaXml
{
public:
	OptimaElement();

	OptimaElement(QGraphicsItem *_item, const QString &itemUuid);
	

	~OptimaElement()
	{
	}

	///обеспечить отрисовку элемента н асхеме
	virtual void draw(bool isProcessLoading = false) = 0;

	///Принять новый xml
	void applyXml(const QDomNode & element, const QDomNode & elementPattern);


	/// эта функция вызывается когда маркер привязанный к этому элементу перемещается пользователем
	virtual void onMarkerMove(const OptimaBaseMarker* marker) = 0;

	virtual void setLinkedHighlight(bool enabled, const QPointF & scenePos = QPointF()) = 0;

	virtual bool checkLinkedHighlight(const QPointF & scenePos) = 0;

	virtual QPointF getIntersectPoint(const QLineF line) const = 0;

	QString uuid() const;

	static void setXmlZOrder(QDomNode & node, qreal sceneZOrder);
	void debugShowXml();
protected:	

	static QDomElement nodeXml(QDomNode node, const QString & name);


private:
	QGraphicsItem *mItem;///<Указатель на графический объект - предок

	///Изменить общие переменные для всех графических элементов согласно xml - описателю
	void applyCommonProperties();

};


