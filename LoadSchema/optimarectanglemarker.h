#pragma once
#include "optimaview.h"
#include "QGraphicsItem"


class OptimaRectangleMarker : public QGraphicsRectItem, public OptimaBaseMarker
{
	Q_DISABLE_COPY( OptimaRectangleMarker )

public:
	OptimaRectangleMarker( QGraphicsItem *parent, Qt::CursorShape cursorShape, const OptimaView *view )
		: OptimaBaseMarker( cursorShape )
		, QGraphicsRectItem(parent)
		, mView(view)
		
	{
	}

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

		OptimaElement *element = dynamic_cast<OptimaElement*>(this->parentItem());
		
		Q_ASSERT(element != nullptr);

		element->onMarkerMove( this );
	}

protected:
	/// указатель на объект, обеспечивающий отрисовку схемы
	const OptimaView* mView;


private:

};

