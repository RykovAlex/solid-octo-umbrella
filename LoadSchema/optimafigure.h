#pragma once
#include "optimaelement.h"
#include <QtXml/QDOMDocument>
#include "optimapoint.h"
#include "optimabasemarker.h"

class OptimaFigure : public QGraphicsPathItem, public OptimaElement
{
public:
	OptimaFigure(const QString &itemUuid, OptimaView *view);

	~OptimaFigure()
	{
	}

	///��������� �������� ��������� ����� �������� xml
	void apply();

	///������ ������� ������
	void draw(bool isProcessLoading = false);


	virtual void onMarkerMove(const OptimaBaseMarker* marker);


	virtual void onHoverEnter(QGraphicsSceneHoverEvent *event);


	virtual void onHoverLeave(QGraphicsSceneHoverEvent* hoverEvent);

protected:

private:

	OptimaPointVector mPoints;///< �������������� ����� ������, ��� ��������� �� �����

	OptimaPointVector mOriginalPoints;///< ��������� ����� ������ ��� ��������������

	OptimaPoint mPositionPoint;///< ������� ������

	qreal mScaleX;///< ����������� ��������������� �� ��� X

	qreal mScaleY;///< ����������� ��������������� �� ��� Y

	///������� ������� ������ �� ������� ������� ����������
	QLineF createLineToCurve(int iStart, int iEnd) const;

	///��������� ��������������� ������ �������� ������������ �� ��������� <kx> � ����������� <ky>
	void scale();
};


