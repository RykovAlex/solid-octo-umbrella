#pragma once
#include "optimafigure.h"

class OptimaLine;
class OptimaConnectorPathFinder
{
public:
	OptimaConnectorPathFinder();

	~OptimaConnectorPathFinder();
	
	static const int margin  = 10; ///< размер отступа от  края описывающего прямоугольника

	enum LineDirection 
	{
		LineDirection_Horizontal
		,LineDirection_Vertical
		,LineDirection_Diagonal
	};

	static QRectF getMarkerRect(QPointF scenePos);

	static QGraphicsItem *findLinkedItem(const QGraphicsScene * scene, const QPointF & scenePos);
	
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

	static OptimaPointVector GetResizableConnectorPoints_Free_Figure(OptimaPointVector param1, int param2, QRectF param3);

	static OptimaPointVector GetResizableConnectorPoints_Free_Free(const OptimaPointVector points, int indexLine);
	static OptimaPointVector GetResizableConnectorPoints_Free_Connector(OptimaPointVector param1, int param2, OptimaLine linkedLine);
	static OptimaPointVector GetResizableConnectorPoints_Connector_Free(OptimaPointVector points, int indexLine, OptimaLine linkedLine);
	static OptimaPointVector GetResizableConnectorPoints_Figure_Free(OptimaPointVector points, int indexLine, QRectF boundingRect);
	static OptimaPointVector GetResizableConnectorPoints_Figure_Connector(OptimaPointVector param1, int param2, QRectF param3, OptimaLine linkedLineEnd);
protected:

private:

public:
	static OptimaPointVector GetResizableConnectorPoints_Connector_Connector(OptimaPointVector param1, int param2, OptimaLine linkedLineBegin, OptimaLine linkedLineEnd);
	static OptimaPointVector GetResizableConnectorPoints_Figure_Figure(OptimaPointVector param1, int param2, QRectF param3, QRectF param4);
	static OptimaPointVector GetResizableConnectorPoints_Connector_Figure(OptimaPointVector param1, int param2, OptimaLine linkedLineBegin, QRectF param4);
};


