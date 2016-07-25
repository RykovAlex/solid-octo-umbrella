#pragma once
#include "optimaview.h"
#include "QGraphicsItem"


class OptimaRectangleMarker : public QGraphicsRectItem, public OptimaBaseMarker
{
	Q_DISABLE_COPY( OptimaRectangleMarker )

public:
	OptimaRectangleMarker( QGraphicsItem *parent, QPointF pos, Qt::CursorShape cursorShape, const OptimaView *view );

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

	inline void markerMoveEvent(const QPointF & newPos)
	{
		setMarkerPos( newPos );

		//не будем перемещать существующий маркер
		//OptimaElement *element = dynamic_cast<OptimaElement*>(this->parentItem());
		//
		//Q_ASSERT(element != nullptr);

		//element->onMarkerMove( this );
	}


	virtual bool isPosChanged(QPointF & pos)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void setLinked(bool val);
protected:
	/// указатель на объект, обеспечивающий отрисовку схемы
	const OptimaView* mView;



	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);


	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:

};

