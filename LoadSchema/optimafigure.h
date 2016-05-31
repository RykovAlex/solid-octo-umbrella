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

	///Применить изенения переданне через струтуру xml
	void apply(const QDomNode & figure);


protected:

private:
	QVector<OptimaPoint> mPoints;///< действительные точки фигуры, после преобразования

	QVector<OptimaPoint> mOriginalPoints;///< начальные точки фигуры для преобразования

	OptimaPoint mPositionPoint;///< позиция фигуры

	qreal mScaleX;///< коэффициент масштабирования по оси X

	qreal mScaleY;///< коэффициент масштабирования по оси Y

	///Задать контуры фигуры
	void applyPath();

	///Создает отрезок фигуры за минусом радиуса скругления
	QLineF createLineToCurve(int iStart, int iEnd) const;

	///Обеспечим масштабирование фигуры согласно коэффицентам по вертикали <kx> и горизонтали <ky>
	void scale();
};


