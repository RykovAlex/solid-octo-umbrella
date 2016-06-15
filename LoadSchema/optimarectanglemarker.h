#pragma once
#include "optimaview.h"
#include "QGraphicsItem"


class OptimaRectangleMarker : public QGraphicsRectItem, public OptimaBaseMarker
{
	Q_DISABLE_COPY( OptimaRectangleMarker )

public:
	OptimaRectangleMarker( Qt::CursorShape cursorShape )
		: OptimaBaseMarker( cursorShape )
		, mView(nullptr)
	{
		//получим указатель на объект обеспечивающий отображение схемы
		QList<QGraphicsView*> views(this->scene()->views());
		for (int i = 0; i < views.count(); ++i)
		{
			mView = dynamic_cast<OptimaView*>(this->scene()->views().at(i));

			if (mView != nullptr)
			{
				break;
			}
		}

		setZValue( 0.999999 );
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
	OptimaView* mView;


private:

};

