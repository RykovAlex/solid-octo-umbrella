#pragma once
#include "optimapoint.h"

class OptimaConnectorPathFinder
{
public:
	OptimaConnectorPathFinder();

	~OptimaConnectorPathFinder();
	
	///Свободная точка - Свободная точка 
	static OptimaPointVector GetNewConnectorPoints_Free_Free(OptimaPoint beginPoint, OptimaPoint endPoint);

protected:

private:

};


