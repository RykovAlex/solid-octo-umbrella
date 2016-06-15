#pragma once
#include "OptimaElement.h"
#include "QGraphicsItem"

class OptimaText: public QGraphicsTextItem, public OptimaElement
{
public:
	OptimaText(const QString &itemUuid, OptimaView *view);

	~OptimaText()
	{
	}


	virtual void apply();

	virtual void draw(bool isProcessLoading = false);

	virtual void onMarkerMove(const OptimaBaseMarker* marker);

	virtual void onHoverEnter(QGraphicsSceneHoverEvent *event);


	virtual void onHoverLeave(QGraphicsSceneHoverEvent* hoverEvent);

protected:

private:
	OptimaPoint mPositionPoint; ///< расположение тектовой метки на схеме

	QString mText; ///< тект метки

	QString mHalign; ///< выравнивание метки

	qreal mTextWidth; ///< ширина тектовой метки
};


