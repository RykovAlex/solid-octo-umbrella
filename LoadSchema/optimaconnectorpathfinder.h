#pragma once
#include "optimapoint.h"

class OptimaConnectorPathFinder
{
public:
	OptimaConnectorPathFinder();

	~OptimaConnectorPathFinder();
	
	///��������� ����� - ��������� ����� 
	static OptimaPointVector GetNewConnectorPoints_Free_Free(OptimaPoint beginPoint, OptimaPoint endPoint);

protected:

private:

};


