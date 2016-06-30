#pragma once
#include "OptimaElement.h"
#include "QGraphicsItem"
#include "tag.h"

class OptimaText: public QGraphicsTextItem, public OptimaElement
{
public:
	enum{ Type = UserType + tag::element::text };

	OptimaText(const QString &itemUuid, OptimaView *view);

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

protected:

private:
	OptimaPoint mPositionPoint; ///< ������������ �������� ����� �� �����

	QString mText; ///< ���� �����

	QString mHalign; ///< ������������ �����

	qreal mTextWidth; ///< ������ �������� �����
};


