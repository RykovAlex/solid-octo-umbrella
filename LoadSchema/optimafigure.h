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

protected:

public:
	///��������� �������� ��������� ����� �������� xml
	void apply(const QDomNode & figure);

	///��������� ��������� ����� � ��������� ��� ���������� ���������
	void parseStructureDot(const QDomNode &figure);

private:
	QVector<OptimaPoint> mPoints;///< �������������� ����� ������, ����� ��������������

	QVector<OptimaPoint> mOriginalPoints;///< ��������� ����� ������ ��� ��������������

	void applyPath();

	///������� ������� ������ �� ������� ������� ����������
	QLineF createLine(int iStart, int iEnd) const;

};


