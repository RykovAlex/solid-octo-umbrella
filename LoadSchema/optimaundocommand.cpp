#include "stdafx.h"
#include "optimaelement.h"
#include "optimaundocommand.h"
#include "optimaconnector.h"

ChangeGeometryConnector::ChangeGeometryConnector(OptimaConnector* connector, const OptimaPointVector &newPoint, int changeIndex) : OptimaUndoCommand(connector, connector->scene())
	, mNewPoint(newPoint)
	, mOldPoint(connector->points())
	, mChangeIndex(changeIndex)
{

}

ChangeGeometryConnector::~ChangeGeometryConnector()
{

}

void ChangeGeometryConnector::undo()
{
	OptimaConnector *connector = getElement<OptimaConnector>();
	
	if (connector != nullptr)
	{
		connector->setPoints(mOldPoint, true);
	}
}

void ChangeGeometryConnector::redo()
{
	OptimaConnector *connector = getElement<OptimaConnector>();

	if (connector != nullptr)
	{
		connector->setPoints(mNewPoint, true);
	}
}

int ChangeGeometryConnector::id() const
{
	return Id;
}

bool ChangeGeometryConnector::mergeWith(const QUndoCommand *other)
{
	const ChangeGeometryConnector *otherUndo = dynamic_cast<const ChangeGeometryConnector*>(other);

	//будем объединять изменения только отдельных элементов коннектора
	if ( !isSame(otherUndo) || mChangeIndex != otherUndo->mChangeIndex || mChangeIndex == -1)
	{
		return false;
	}
	mNewPoint = otherUndo->getElement<OptimaConnector>()->points();

	return true;
}
