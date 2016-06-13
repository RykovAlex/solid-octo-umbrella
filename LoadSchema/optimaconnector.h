#pragma once
#include "optimaelement.h"
#include <QtXml/QDOMDocument>
#include "optimapoint.h"
#include "optimaconnectorarrow.h"
#include "optimapath.h"

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
	void draw(bool isProcessLoading = false);

	void buildPath(const OptimaCross & cross);

	void getIntersection(const QList<QGraphicsItem*> &itemList, int start);

	void clearIntersection();
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
	
	OptimaPath mConnectorPath;

	bool mIsHighlight;

	QPen mPen;

	void intersected(OptimaPath & connectorPath);
};


