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

	void splitOnPieces();

	void addShapePieces(const QPointF centerPoint, const QLineF &rectSegment);


	virtual void onMarkerMove(const OptimaBaseMarker* marker);

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

	void setHighlightShapePieces(const QPointF & scenePos, bool enabled);
protected:

private:

	OptimaPointVector mPoints;///< �������������� ����� ������, ��� ��������� �� �����

	OptimaPointVector mOriginalPoints;///< ��������� ����� ������ ��� ��������������

	OptimaPoint mPositionPoint;///< ������� ������

	qreal mScaleX;///< ����������� ��������������� �� ��� X

	qreal mScaleY;///< ����������� ��������������� �� ��� Y

	QVector<QPolygonF> shapePieces;///<����� ������� �������� boundingRect ��� ����������� ������� ������������� ����������
	

	///������� ������� ������ �� ������� ������� ����������
	QLineF createLineToCurve(int iStart, int iEnd) const;

	///��������� ��������������� ������ �������� ������������ �� ��������� <kx> � ����������� <ky>
	void scale();

	int mIndexShapePieces;
};


