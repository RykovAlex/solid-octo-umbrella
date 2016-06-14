#pragma once
#include <QtGui/QGraphicsView>
#include <QtXml/QDOMDocument>
#include "OptimaElement.h"

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

	inline virtual void markerMoveEvent(const OptimaBaseMarker* marker)
	{
		Q_ASSERT(false);
	}

private:
	QDomDocument doc;	
	
	qreal mAlignGridStep;

	virtual void apply();

	virtual void draw(bool isProcessLoading = false);

	void beforeExecute1CCall();

	///Загружает файл XML - описателя сцены и создает / изменяет ее графическое отображение
	template <class T> void loadElements(const QDomNodeList &elements, bool loadAllways);

	QGraphicsItem *findItem(const QString &itemUuid);
	
	///Найдем на схеме элемент с itemUuid из xml, если его нет, то зоздадим его
	template <class T>
	T *getItem(const QString &itemUuid);

	QString getUuid(QGraphicsItem* item);
	
	void buildIntersectionConnectors();
	
	void loadWorkspace(const QDomNodeList &workspace);
	
};