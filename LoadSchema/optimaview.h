#pragma once
#include <QtGui/QGraphicsView>
#include <QtXml/QDOMDocument>
#include "OptimaElement.h"
#include "optimafigure.h"
#include "optimatemporaryconnector.h"
#include "optimaconnectorpathfinder.h"

class OptimaConnector;
class OptimaTemporaryConnector;
class OptimaView : public QGraphicsView, public OptimaElement
{
	Q_OBJECT;

public Q_SLOTS:
		QString LoadScheme(const QString &filename, bool load_allways);


Q_SIGNALS:
		void ErrorOccur( const QString & text );

public:
	enum Mode { InsertItem, InsertLine, InsertText, MoveItem };

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

	void setMode(Mode mode);

	virtual void setLinkedHighlight(bool enabled, const QPointF & scenePos = QPointF() );

	virtual bool checkLinkedHighlight(const QPointF & scenePos);

	void buildIntersectionConnectors();
	
	void addConnector(OptimaConnector * oldConnector, bool reversed);

	virtual QPointF getIntersectPoint(const QLineF line) const;

protected:

	virtual void mousePressEvent(QMouseEvent *mouseEvent);

	void addConnector(QMouseEvent * mouseEvent);
	virtual void mouseReleaseEvent(QMouseEvent *mouseEvent);

	virtual void mouseMoveEvent(QMouseEvent *mouseEvent);

	void setMarkerPen(QGraphicsRectItem * borderEnd, QPointF scenePos);

	void updateHighlightLinkedElement(QPointF scenePos);

	void updateHighlightStartLinkedElement();
	
	virtual void keyPressEvent(QKeyEvent *keyEvent);

private:
	QDomDocument doc;///<Этот объект сохраняет всю xml-структуру 
	
	qreal mAlignGridStep;///<Шаг виртуальной сетки

	Mode mMode;///<текущее активное действие сцены (создаем коннектор, фигуру, что-нибудь передвигаем)
	
	OptimaTemporaryConnector *mNewConnector;///<отображение для коннетора который только еще создается

	OptimaConnector *mOldConnector;///<здесь храним коннетор который изменяем
	
	OptimaElement *mLinkedBeginElement;///<хранит указатель на фигуру, котороую подсветили при начале движения коннеткора
	
	OptimaElement *mLinkedEndElement;///<хранит указатель на фигуру, которую подсветили для присоединения коннектора

	virtual void apply();

	virtual void draw(bool isProcessLoading = false);

	template <class T> void loadElement(const QDomNode &element, bool loadAllways);

	void beforeExecute1CCall();

	qreal findNextZOrder();
	///Загружает файл XML - описателя сцены и создает / изменяет ее графическое отображение
	template <class T> void loadElements(const QDomNodeList &elements, bool loadAllways);

	///Найти элемент на схеме по его uuid
	QGraphicsItem *findItem(const QString &itemUuid);
	
	///Найти элемент на схеме по абсолютной координате
	OptimaElement * findItem( const QPoint & pos );
	
	///Найдем на схеме элемент с itemUuid из xml, если его нет, то создать его
	template <class T>
	T *getItem(const QString &itemUuid);

	QString getUuid(QGraphicsItem* item);
	
	void loadWorkspace(const QDomNodeList &workspace);

	inline OptimaElement * getLinkedElement(const QPointF & scenePos)
	{
		return dynamic_cast<OptimaElement*>(OptimaConnectorPathFinder::findLinkedItem(scene(), scenePos));
	}
	
	QDomNode createEmptyConnectorXmlNode(OptimaTemporaryConnector *tempConnector);

	void onEndDragConnector();

	void onRebuildConnector();

	void onCreateConnector();

};
