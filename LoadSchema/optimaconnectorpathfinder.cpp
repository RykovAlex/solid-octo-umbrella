#include "stdafx.h"
#include "optimaconnectorpathfinder.h"



OptimaConnectorPathFinder::OptimaConnectorPathFinder()
{

}

OptimaConnectorPathFinder::~OptimaConnectorPathFinder()
{

}

OptimaPointVector OptimaConnectorPathFinder::GetNewConnectorPoints_Free_Free(OptimaPoint beginPoint, OptimaPoint endPoint)
{
	OptimaPointVector pointVector;

	pointVector << beginPoint << endPoint;

	return pointVector;
}
