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
	///Применить изенения переданне через струтуру xml
	void apply(const QDomNode & figure);

	///Разобрать структуру точек и сохранить для дальнейшей обработки
	void parseStructureDot(const QDomNode &figure);

private:
	QVector<OptimaPoint> mPoints;///< действительные точки фигуры, после преобразования

	QVector<OptimaPoint> mOriginalPoints;///< начальные точки фигуры для преобразования

	void applyPath();

	///Создает отрезок фигуры за минусом радиуса скругления
	QLineF createLine(int iStart, int iEnd) const;

};


