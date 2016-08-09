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
		, baseWidth(1 * 6.0)
	{
	}

	~OptimaBaseMarker()
	{
	}

	/// ������ �������
	//QCursor getMarkerCursor() const
	//{
	//	switch(mCursor.shape())
	//	{
	//	case Qt::SizeAllCursor:
	//		if ( linked() ) 
	//		{
	//			return QCursor( QPixmap( ":/images/cursor_move_all_red.png" , 0, 0 ));	
	//		}
	//	default:
	//		return mCursor;
	//	}
	//}
	
	/// ���������� �������� �� �������, � ���� ��, �� ���������� ����� �������
	virtual bool isPosChanged( QPointF & pos ) = 0;
	
	/// �������� ������� ��� ����� �������
	virtual void setMarkerPos( const QPointF & pos ) = 0;
	
	/// �������� ������� ������� �����
	virtual QPointF markerPos( ) = 0;

	/// ������ �������
	const qreal baseWidth;

	/// ����������� �� ������������ ��� ������ ��������
	bool isMovementBlocked;
	
	/// ������ �������
	const QCursor mCursor;
	
protected:
	
	QPointF deltaPos; ///<������ ������������� ������� ������������ �������

private:

};


