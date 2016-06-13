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
	void apply();

	///Задать контуры фигуры
	void draw(bool isProcessLoading = false);

protected:

private:

	OptimaPointVector mPoints;///< действительные точки фигуры, для отрисовке на сцене

	OptimaPointVector mOriginalPoints;///< начальные точки фигуры для преобразования

	OptimaPoint mPositionPoint;///< позиция фигуры

	qreal mScaleX;///< коэффициент масштабирования по оси X

	qreal mScaleY;///< коэффициент масштабирования по оси Y

	///Создает отрезок фигуры за минусом радиуса скругления
	QLineF createLineToCurve(int iStart, int iEnd) const;

	///Обеспечим масштабирование фигуры согласно коэффицентам по вертикали <kx> и горизонтали <ky>
	void scale();
};


