#pragma once
#include "QCursor"
#include "QPixmap"

class OptimaBaseMarker
{
	Q_DISABLE_COPY( OptimaBaseMarker )

public:
	OptimaBaseMarker( const Qt::CursorShape cursorShape )
		: mCursor( cursorShape )
		, isMovementBlocked( true )
		, linked( false )
	{
	}

	~OptimaBaseMarker()
	{
	}

	/// курсор маркера
	QCursor getMarkerCursor() const
	{
		switch(mCursor.shape())
		{
		case Qt::SizeAllCursor:
			if ( linked ) 
			{
				return QCursor( QPixmap( ":/images/cursor_move_all_red.png" , 0, 0 ));	
			}
		default:
			return mCursor;
		}
	}
	
	/// определяем сменится ли позиция, и если да, то возвращает новую позицию
	virtual bool isPosChanged( QPointF & pos ) = 0;
	
	/// сигнализирует что маркер подвинули
	virtual void markerMoveEvent( const QPointF & pos ) = 0;
	
	/// передвигаем итем
	virtual void setMarkerPos( const QPointF & pos ) = 0;
	
	/// сообщаем текущую позицию итема
	virtual QPointF markerPos( ) = 0;

	
	/// блокировать ли сигнализация что маркер двинулся
	bool isMovementBlocked;
	
	/// курсор маркера
	const QCursor mCursor;
	
	/// признак того, что этот маркер отображает присоединение
	bool linked;

protected:

private:

};


