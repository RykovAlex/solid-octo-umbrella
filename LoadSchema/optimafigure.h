#pragma once
#include "optimaelement.h"
#include <QtXml/QDOMDocument>
#include "optimapoint.h"

class OptimaFigure : public QGraphicsPathItem, public OptimaElement
{
public:
	OptimaFigure(const QString &itemUuid);

	~OptimaFigure()
	{
	}

	///��������� �������� ��������� ����� �������� xml
	void apply(const QDomNode & figure);


protected:

private:
	QVector<OptimaPoint> mPoints;///< �������������� ����� ������, ����� ��������������

	QVector<OptimaPoint> mOriginalPoints;///< ��������� ����� ������ ��� ��������������

	OptimaPoint mPositionPoint;///< ������� ������

	qreal mScaleX;///< ����������� ��������������� �� ��� X

	qreal mScaleY;///< ����������� ��������������� �� ��� Y

	///������ ������� ������
	void applyPath();

	///������� ������� ������ �� ������� ������� ����������
	QLineF createLineToCurve(int iStart, int iEnd) const;

	///��������� ��������������� ������ �������� ������������ �� ��������� <kx> � ����������� <ky>
	void scale();
};


