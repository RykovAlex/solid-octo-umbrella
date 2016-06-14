#pragma once
#include "OptimaElement.h"
#include "QGraphicsItem"

class OptimaText: public QGraphicsTextItem, public OptimaElement
{
public:
	OptimaText(const QString &itemUuid);

	~OptimaText()
	{
	}


	virtual void apply();

	virtual void draw(bool isProcessLoading = false);

	virtual void markerMoveEvent(const OptimaBaseMarker* marker);

protected:

private:
	OptimaPoint mPositionPoint; ///< ������������ �������� ����� �� �����

	QString mText; ///< ���� �����

	QString mHalign; ///< ������������ �����

	qreal mTextWidth; ///< ������ �������� �����
};


