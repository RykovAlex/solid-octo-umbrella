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
	OptimaPoint mPositionPoint; ///< ������������ �������� ����� �� �����

	QString mText; ///< ���� �����

	QString mHalign; ///< ������������ �����

	qreal mTextWidth; ///< ������ �������� �����
};


