#include "stdafx.h"
#include "optimaconnectormovemarker.h"

OptimaConnectorMoveMarker::OptimaConnectorMoveMarker(Qt::CursorShape cursorShape) 
	: OptimaRectangleMarker(cursorShape)
{
}

bool OptimaConnectorMoveMarker::isPosChanged(QPointF & newPos)
{
	Q_ASSERT(mView != nullptr);

	if ( isMovementBlocked )
	{
		return false;
	}

	//маркер передвигается только по виртуальной сетке
	const QPointF currentPos = this->pos();
	const QPoint currrentAlignedPos(mView->alignToGrid(currentPos).toPoint());
	const QPoint newAlignedPos(mView->alignToGrid( newPos ).toPoint());

	QPointF resultPos( currentPos );

	//определим как двигается маркер
	switch(mCursor.shape())
	{
	case Qt::SizeHorCursor:
		resultPos.setX( newAlignedPos.x( ) );
		break;
	case Qt::SizeVerCursor:
		resultPos.setY( newAlignedPos.y( ) );
		break;
	case Qt::SizeAllCursor:
		resultPos.setX( newAlignedPos.x( ) );
		resultPos.setY( newAlignedPos.y( ) );
		break;
	default:
		Q_ASSERT( false );
	}

	//проверим произошло ли передвижение маркера с точки зрения виртуальной сетки
	const QPoint resultAlignedPos(mView->alignToGrid(resultPos).toPoint());
	if ( resultAlignedPos == currrentAlignedPos )
	{
		return false;
	}

	newPos = resultPos;

	return true;
}
