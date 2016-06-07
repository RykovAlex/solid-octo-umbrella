#pragma once
#include "optimaelement.h"
#include <QtXml/QDOMDocument>
#include "optimapoint.h"
#include "optimaconnectorarrow.h"

class OptimaConnector : public QGraphicsPathItem, public OptimaElement
{
public:
	OptimaConnector(const QString &itemUuid);

	~OptimaConnector()
	{
	}
	
	///Применить изенения переданне через струтуру xml
	void apply();

	//Рисует коннетор, точнее формирует путь, которй добавляется на схему
	void draw();

protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		QWidget *widget = 0);

private:
	OptimaPointVector mPoints;///< действительные точки фигуры, для отрисовке на сцене

	OptimaConnectorArrow mBeginArrow;///< стрелка в начале коннектора

	OptimaConnectorArrow mEndArrow;///< стрелка в конце коннектора

	OptimaCross mCross;///< класс отвечает за отрисовку пересечений с другими коннеторами
	
	qreal mRadiusCorner;///< радиус скругления углов коннетора
	
	QPainterPath mPathArrow;
	
	qreal getCircleRadius(const int indexCorner) const;
	
	void drawCorner( QPainterPath &path, const QLineF originalLine, const int indexCorner ) const;

	bool mIsHighlight;
	QPen mPen;
public:
	void buildIntersection(const QList<QGraphicsItem*> &itemList);
};


