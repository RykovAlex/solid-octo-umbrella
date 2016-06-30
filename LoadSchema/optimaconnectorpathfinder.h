#pragma once
#include "optimafigure.h"
#include "optimaline.h"

class OptimaConnectorPathFinder
{
public:
	OptimaConnectorPathFinder();

	~OptimaConnectorPathFinder();
	
	static const int margin  = 10; ///< размер отступа от  края описывающего прямоугольника

	static QGraphicsItem * findLinkedItem(const QGraphicsScene * scene,  const QPointF & scenePos);
	
	///Свободная точка - Свободная точка 
	static OptimaPointVector GetNewConnectorPoints_Free_Free(const OptimaPoint beginPoint, const  OptimaPoint endPoint);

	///Свободная точка - Фигура
	static OptimaPointVector GetNewConnectorPoints_Free_Figure(const OptimaPoint beginPoint, const OptimaLinkedRect linkedRect);

	///Свободная точка - Коннектор
	static OptimaPointVector GetNewConnectorPoints_Free_Connector(const OptimaPoint beginPoint, const OptimaLine lineConnector, const OptimaPoint endPoint);

	///Фигура - Свободная точка
	static OptimaPointVector GetNewConnectorPoints_Figure_Free(OptimaLinkedRect param1, QPointF scenePos);

	///Фигура - Фигура
	static OptimaPointVector GetNewConnectorPoints_Figure_Figure(const OptimaLinkedRect beginLinkedRect, const OptimaLinkedRect endLinkedRect);

	///Фигура - Коннектор
	static OptimaPointVector GetNewConnectorPoints_Figure_Connector(const OptimaLinkedRect beginLinkedRect, const OptimaLine endLineConnector, const OptimaPoint endPoint);


	///Коннектор - Свободная точка
	static OptimaPointVector GetNewConnectorPoints_Connector_Free(const OptimaLine lineConnector, const OptimaPoint beginPoint, const OptimaPoint endPoint);

	///Коннектор - Фигура
	static OptimaPointVector GetNewConnectorPoints_Connector_Figure(const OptimaLine lineConnector, const OptimaPoint beginPoint, const OptimaLinkedRect linkedRect);
	
	///Свободная Коннектор - Коннектор
	static OptimaPointVector GetNewConnectorPoints_Connector_Connector(const OptimaLine beginLineConnector, const OptimaPoint beginPoint, const OptimaLine endLineConnector, const OptimaPoint endPoint);

protected:


private:


};


