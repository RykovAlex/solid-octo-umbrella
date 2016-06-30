#pragma once
#include "optimafigure.h"
#include "optimaline.h"

class OptimaConnectorPathFinder
{
public:
	OptimaConnectorPathFinder();

	~OptimaConnectorPathFinder();
	
	static const int margin  = 10; ///< ������ ������� ��  ���� ������������ ��������������

	static QGraphicsItem * findLinkedItem(const QGraphicsScene * scene,  const QPointF & scenePos);
	
	///��������� ����� - ��������� ����� 
	static OptimaPointVector GetNewConnectorPoints_Free_Free(const OptimaPoint beginPoint, const  OptimaPoint endPoint);

	///��������� ����� - ������
	static OptimaPointVector GetNewConnectorPoints_Free_Figure(const OptimaPoint beginPoint, const OptimaLinkedRect linkedRect);

	///��������� ����� - ���������
	static OptimaPointVector GetNewConnectorPoints_Free_Connector(const OptimaPoint beginPoint, const OptimaLine lineConnector, const OptimaPoint endPoint);

	///������ - ��������� �����
	static OptimaPointVector GetNewConnectorPoints_Figure_Free(OptimaLinkedRect param1, QPointF scenePos);

	///������ - ������
	static OptimaPointVector GetNewConnectorPoints_Figure_Figure(const OptimaLinkedRect beginLinkedRect, const OptimaLinkedRect endLinkedRect);

	///������ - ���������
	static OptimaPointVector GetNewConnectorPoints_Figure_Connector(const OptimaLinkedRect beginLinkedRect, const OptimaLine endLineConnector, const OptimaPoint endPoint);


	///��������� - ��������� �����
	static OptimaPointVector GetNewConnectorPoints_Connector_Free(const OptimaLine lineConnector, const OptimaPoint beginPoint, const OptimaPoint endPoint);

	///��������� - ������
	static OptimaPointVector GetNewConnectorPoints_Connector_Figure(const OptimaLine lineConnector, const OptimaPoint beginPoint, const OptimaLinkedRect linkedRect);
	
	///��������� ��������� - ���������
	static OptimaPointVector GetNewConnectorPoints_Connector_Connector(const OptimaLine beginLineConnector, const OptimaPoint beginPoint, const OptimaLine endLineConnector, const OptimaPoint endPoint);

protected:


private:


};


