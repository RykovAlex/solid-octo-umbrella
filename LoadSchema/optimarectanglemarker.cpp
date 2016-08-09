#include "stdafx.h"
#include "optimarectanglemarker.h"
#include "tag.h"
#include "optimafigure.h"
#include "optimaconnector.h"
#include "OptimaConnectorMarker.h"
#include "optimaconnectorbordermarkerbegin.h"
#include "optimaconnectorbordermarkerend.h"
#include "optimascene.h"

OptimaRectangleMarker::OptimaRectangleMarker(QGraphicsItem *parent, QPointF pos, Qt::CursorShape cursorShape, OptimaScene *optimaScene) : OptimaBaseMarker( cursorShape )
	, QGraphicsRectItem(parent)
	, mOptimaScene(optimaScene)
{
	qreal borderMarkerWidth = baseWidth/* * (3.  / m_workspace.m_scale)*/;

	setRect( QRectF( -borderMarkerWidth / 2, -borderMarkerWidth / 2, borderMarkerWidth, borderMarkerWidth ) );

	setBrush( Qt::white );
	isMovementBlocked = false;

	setPos(pos);
	deltaPos = pos;
	setCursor(cursorShape);

	setFlag(ItemIsMovable);
}

void OptimaRectangleMarker::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	deltaPos = QPointF();
}

void OptimaRectangleMarker::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	//маркер передвигается выровненным по сетке
	deltaPos += event->scenePos() - event->lastScenePos();
	QPointF alignedDeltaPos = mOptimaScene->alignToGrid(deltaPos);

	//определим как двигается маркер
	switch(mCursor.shape())
	{
	case Qt::SizeHorCursor:
		alignedDeltaPos.setY( 0.0 );
		break;
	case Qt::SizeVerCursor:
		alignedDeltaPos.setX( 0.0 );
		break;
	case Qt::SizeAllCursor:
		break;
	default:
		Q_ASSERT( false );
	}

	if (alignedDeltaPos.isNull())
	{
		return;
	}

	deltaPos -= alignedDeltaPos;
	QPointF newPos(pos() + alignedDeltaPos);

	setMarkerPos( newPos );
	//markerMoveEvent(newPos);
}

