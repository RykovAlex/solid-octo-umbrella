#pragma once
#include "optimaelement.h"
#include <QtXml/QDOMDocument>
#include "optimapoint.h"
#include "optimabasemarker.h"
#include "tag.h"

class OptimaLinkedRect : public QRectF
{
public:
	enum rectSide {
		top		= 0
		,right
		,bottom
		,left	
	};
	OptimaLinkedRect(const QRectF &rect, const rectSide side);

	rectSide side() const;

private:
	rectSide mSide;
};

class OptimaFigure : public QGraphicsPathItem, public OptimaElement
{
public:
	enum { Type = UserType + tag::element::figure };

	OptimaFigure(const QString &itemUuid);

	~OptimaFigure()
	{
	}

	///Применить изенения переданне через струтуру xml
	void apply();

	///Задать контуры фигуры
	void draw(bool isProcessLoading = false);

	void splitOnPieces();

	void addShapePieces(const QPointF centerPoint, const QLineF &rectSegment);

	virtual void setLinkedHighlight(bool enabled, const QPointF & scenePos = QPointF());

	virtual void onMarkerMove(const OptimaBaseMarker* marker);

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

	virtual int type() const;

	OptimaLinkedRect::rectSide edgePosition() const { return OptimaLinkedRect::rectSide(mIndexShapePieces); }

	inline OptimaLinkedRect linkedRect() const
	{
		return OptimaLinkedRect(sceneBoundingRect(), edgePosition());
	}


	bool checkLinkedHighlight(const QPointF & scenePos, int & indexShapePieces);

	virtual bool checkLinkedHighlight(const QPointF & scenePos);


	virtual QPointF getIntersectPoint(const QLineF line) const;

protected:

private:

	OptimaPointVector mPoints;///< действительные точки фигуры, для отрисовке на сцене

	OptimaPointVector mOriginalPoints;///< начальные точки фигуры для преобразования

	OptimaPoint mPositionPoint;///< позиция фигуры

	qreal mScaleX;///< коэффициент масштабирования по оси X

	qreal mScaleY;///< коэффициент масштабирования по оси Y

	QVector<QPolygonF> shapePieces;///<будем хранить элементы boundingRect для определения области присоединения коннектора
	

	///Создает отрезок фигуры за минусом радиуса скругления
	QLineF createLineToCurve(int iStart, int iEnd) const;

	///Обеспечим масштабирование фигуры согласно коэффицентам по вертикали <kx> и горизонтали <ky>
	void scale();

	int mIndexShapePieces;
public:
	static QLineF getEdgeLine(const QRectF & sceneBoundingRect, OptimaLinkedRect::rectSide side);
};

inline bool isFigure(const QGraphicsItem* item)
{
	return (qgraphicsitem_cast<const OptimaFigure*>(item) != nullptr);
}

