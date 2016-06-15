#pragma once
#include <QtGui/QGraphicsView>
#include <QtXml/QDOMDocument>
#include "OptimaElement.h"

class OptimaConnector;

class OptimaView : public QGraphicsView, public OptimaElement
{
	Q_OBJECT;

public Q_SLOTS:
		QString LoadScheme(const QString &filename, bool load_allways);


Q_SIGNALS:
		void ErrorOccur( const QString & text );

public:
	OptimaView(QWidget *parent = 0);

	inline int getEntireCellsQnt( qreal coordinate ) const
	{
		return qRound( coordinate / mAlignGridStep );
	}
	
	inline qreal alignToGrid( const qreal & coordinate ) const
	{
		return mAlignGridStep * getEntireCellsQnt( coordinate );
	}
	
	inline QPointF alignToGrid( const QPointF& pos ) const
	{
		return QPointF( alignToGrid( pos.x() ), alignToGrid( pos.y() ) );
	}
	
	inline QPoint alignToGrid( const QPoint& pos ) const
	{
		return QPoint( alignToGrid( pos.x() ), alignToGrid( pos.y() ) );
	}

	inline virtual void onMarkerMove(const OptimaBaseMarker* marker)
	{
		Q_ASSERT(false);
	}


private:
	QDomDocument doc;///<���� ������ ��������� ��� xml-��������� 
	
	qreal mAlignGridStep;///<��� ����������� �����


	
	virtual void apply();

	virtual void draw(bool isProcessLoading = false);

	void beforeExecute1CCall();

	///��������� ���� XML - ��������� ����� � ������� / �������� �� ����������� �����������
	template <class T> void loadElements(const QDomNodeList &elements, bool loadAllways);

	///����� ������� �� ����� �� ��� uuid
	QGraphicsItem *findItem(const QString &itemUuid);
	
	///����� ������� �� ����� �� ���������� ����������
	OptimaElement * findItem( const QPoint & pos );
	
	///������ �� ����� ������� � itemUuid �� xml, ���� ��� ���, �� ������� ���
	template <class T>
	T *getItem(const QString &itemUuid);

	QString getUuid(QGraphicsItem* item);
	
	void buildIntersectionConnectors();
	
	void loadWorkspace(const QDomNodeList &workspace);

	bool isConnector(const QGraphicsItem* item) const;
	
};