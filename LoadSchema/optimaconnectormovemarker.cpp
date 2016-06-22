#include "stdafx.h"
#include "optimaconnectormovemarker.h"

OptimaConnectorMoveMarker::OptimaConnectorMoveMarker(OptimaConnector *parentConnector, const QPointF & pos, Qt::CursorShape cursorShape) 
	//: OptimaRectangleMarker(parentConnector, cursorShape, parentConnector->view())
	: OptimaRectangleMarker(parentConnector, cursorShape, parentConnector->view())
{
	qreal borderMarkerWidth = baseWidth/** (3.  / m_workspace.m_scale)*/;

	setRect( QRectF( -borderMarkerWidth / 2, -borderMarkerWidth / 2, borderMarkerWidth, borderMarkerWidth ) );
	
	QPen pen( Qt::green, 1.0 );
	pen.setCosmetic( true );
	setPen( pen );			

	setBrush( Qt::white );
	isMovementBlocked = false;

	setPos(pos);
	deltaPos = pos;
	setCursor(cursorShape);


	setAcceptHoverEvents(true);
	//setFlag(ItemIsSelectable);
	setFlag(ItemIsMovable);
	//parentConnector->view()->scene()->addItem(this);			
	//setParentItem(nullptr);
	//setZValue(0.999998);
}

bool OptimaConnectorMoveMarker::isPosChanged(QPointF & pos)
{
	return true;
}

QVariant OptimaConnectorMoveMarker::itemChange(GraphicsItemChange change, const QVariant &value)
{
	switch (change)
	{
	case ItemPositionChange:
		{
			return value;
		}
		break;
	case ItemMatrixChange:
		break;
	case ItemVisibleChange:
		break;
	case ItemEnabledChange:
		break;
	case ItemSelectedChange:
		break;
	case ItemParentChange:
		break;
	case ItemChildAddedChange:
		break;
	case ItemChildRemovedChange:
		break;
	case ItemTransformChange:
		break;
	case ItemPositionHasChanged:
		break;
	case ItemTransformHasChanged:
		break;
	case ItemSceneChange:
		break;
	case ItemVisibleHasChanged:
		break;
	case ItemEnabledHasChanged:
		break;
	case ItemSelectedHasChanged:
		break;
	case ItemParentHasChanged:
		break;
	case ItemSceneHasChanged:
		break;
	case ItemCursorChange:
		break;
	case ItemCursorHasChanged:
		break;
	case ItemToolTipChange:
		break;
	case ItemToolTipHasChanged:
		break;
	case ItemFlagsChange:
		break;
	case ItemFlagsHaveChanged:
		break;
	case ItemZValueChange:
		break;
	case ItemZValueHasChanged:
		break;
	case ItemOpacityChange:
		break;
	case ItemOpacityHasChanged:
		break;
	case ItemScenePositionHasChanged:
		break;
	case ItemRotationChange:
		break;
	case ItemRotationHasChanged:
		break;
	case ItemScaleChange:
		break;
	case ItemScaleHasChanged:
		break;
	case ItemTransformOriginPointChange:
		break;
	case ItemTransformOriginPointHasChanged:
		break;
	default:
		break;
	}

	return value;
}

void OptimaConnectorMoveMarker::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
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
	QPointF newPos(itemChange(ItemPositionChange, pos() + alignedDeltaPos).toPointF() );
	setPos( newPos );
}

void OptimaConnectorMoveMarker::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	deltaPos = QPointF();
}
