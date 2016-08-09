#pragma once
#include "QGraphicsItem"
#include "optimabasemarker.h"

class OptimaScene;
class OptimaRectangleMarker : public QGraphicsRectItem, public OptimaBaseMarker
{
	Q_DISABLE_COPY( OptimaRectangleMarker )

public:
	OptimaRectangleMarker( QGraphicsItem *parent, QPointF pos, Qt::CursorShape cursorShape, OptimaScene *optimaScene );

	~OptimaRectangleMarker()
	{
	}

	inline virtual void setMarkerPos( const QPointF & pos )
	{
		setPos( pos );
	}

	inline virtual QPointF markerPos( )
	{
		return pos();
	}

	virtual bool isPosChanged(QPointF & pos)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

protected:
	/// указатель на объект, обеспечивающий отрисовку схемы
	OptimaScene* mOptimaScene;

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:

};

