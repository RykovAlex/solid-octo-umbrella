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
	OptimaPointVector mPoints;///< �������������� ����� ������, ��� ��������� �� �����

	OptimaPointVector mOriginalPoints;///< ��������� ����� ������ ��� ��������������

	OptimaPoint mPositionPoint;///< ������� ������

	qreal mScaleX;///< ����������� ��������������� �� ��� X

	qreal mScaleY;///< ����������� ��������������� �� ��� Y

	///������ ������� ������
	void draw();

	///������� ������� ������ �� ������� ������� ����������
	QLineF createLineToCurve(int iStart, int iEnd) const;

	///��������� ��������������� ������ �������� ������������ �� ��������� <kx> � ����������� <ky>
	void scale();
};


