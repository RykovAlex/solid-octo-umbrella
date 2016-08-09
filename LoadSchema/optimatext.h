#pragma once
#include "OptimaElement.h"
#include "QGraphicsItem"
#include "tag.h"

class OptimaText: public QGraphicsTextItem, public OptimaElement
{
public:
	enum{ Type = UserType + tag::element::text };

	OptimaText(const QString &itemUuid);

	~OptimaText()
	{
	}


	virtual void apply();

	virtual void draw(bool isProcessLoading = false);

	virtual void onMarkerMove(const OptimaBaseMarker* marker);

	virtual void onHoverEnter(QGraphicsSceneHoverEvent *event);


	virtual void onHoverLeave(QGraphicsSceneHoverEvent* hoverEvent);


	virtual int type() const;


	virtual void setLinkedHighlight(bool enabled, const QPointF & scenePos = QPointF());


	virtual bool checkLinkedHighlight(const QPointF & scenePos);


	virtual QPointF getIntersectPoint(const QLineF line) const;

protected:

private:
	OptimaPoint mPositionPoint; ///< расположение тектовой метки на схеме

	QString mText; ///< тект метки

	QString mHalign; ///< выравнивание метки

	qreal mTextWidth; ///< ширина тектовой метки
};


