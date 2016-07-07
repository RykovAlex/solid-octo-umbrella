#include "stdafx.h"
#include "optimarectanglemarker.h"
#include "tag.h"

OptimaRectangleMarker::OptimaRectangleMarker(QGraphicsItem *parent, QPointF pos, Qt::CursorShape cursorShape, const OptimaView *view) : OptimaBaseMarker( cursorShape )
	, QGraphicsRectItem(parent)
	, mView(view)
{
	qreal borderMarkerWidth = baseWidth/* * (3.  / m_workspace.m_scale)*/;

	setRect( QRectF( -borderMarkerWidth / 2, -borderMarkerWidth / 2, borderMarkerWidth, borderMarkerWidth ) );

	setLinked(false);

	setBrush( Qt::white );
	isMovementBlocked = false;

	setPos(pos);
	deltaPos = pos;
	setCursor(cursorShape);


	//setAcceptHoverEvents(true);
	//setFlag(ItemIsSelectable);
	setFlag(ItemIsMovable);
	//parentConnector->view()->scene()->addItem(this);			
	//setParentItem(nullptr);
	//setZValue(0.999998);
}

void OptimaRectangleMarker::setLinked(bool val)
{
	OptimaBaseMarker::setLinked(val);
	
	QPen pen( val ? tag::markerLinkColor : tag::markerFreeColor, 1.0 );
	pen.setCosmetic( true );
	setPen( pen );			
}

void OptimaRectangleMarker::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	deltaPos = QPointF();
}

void OptimaRectangleMarker::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	//маркер передвигается выровненным по сетке
	deltaPos += event->scenePos() - event->lastScenePos();
	QPointF alignedDeltaPos = mView->alignToGrid(deltaPos);

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
	setPos( newPos );
}