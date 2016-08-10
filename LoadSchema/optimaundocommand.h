#pragma once
#include "QUndoStack"
#include "optimapoint.h"
#include "tag.h"

class OptimaConnector;
class OptimaElement;
class OptimaScene;
class OptimaUndoCommand : public QUndoCommand
{
public:
	OptimaUndoCommand(OptimaElement *element, OptimaScene *scene) : mUuid(element->uuid()), mScene(scene) {}
protected:
	template<class T>
	T *getElement() const
	{
		T *element = dynamic_cast<T *>(mScene->findItem(mUuid));
		Q_ASSERT(element != nullptr);
		return element;
	}

	bool isSame(const OptimaUndoCommand *otherUndo) const
	{
		return mUuid == otherUndo->mUuid;
	}
private:
	const QString mUuid;
	OptimaScene *mScene;
};

class ChangeGeometryConnector : public OptimaUndoCommand
{
public:
	enum { Id = tag::undo::change_geometry_connector };
	ChangeGeometryConnector(OptimaConnector* connector, const OptimaPointVector &newPoint, int changeIndex);

	~ChangeGeometryConnector();


	virtual void undo();


	virtual void redo();


	virtual int id() const;


	virtual bool mergeWith(const QUndoCommand *other);

protected:

private:
	OptimaPointVector mNewPoint;
	const OptimaPointVector mOldPoint;
	int mChangeIndex;
};


