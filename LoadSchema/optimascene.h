#pragma once
#include "QGraphicsScene"
#include "optimaxml.h"

class OptimaElement;
class OptimaConnectorMarker;
class OptimaConnectorBorderMarkerEnd;
class OptimaScene: public QGraphicsScene, public OptimaXml
{
	Q_OBJECT;

public Q_SLOTS:
		QString LoadScheme(const QString &xml, const QString &xmlPattern, bool load_allways);


Q_SIGNALS:
		void ErrorOccur( const QString & text );

public:
	enum Mode { InsertItem, InsertLine, InsertText, MoveItem };

	OptimaScene();

	~OptimaScene();

	int getEntireCellsQnt( qreal coordinate ) const;

	qreal alignToGrid( const qreal & coordinate ) const;

	QPointF alignToGrid( const QPointF& pos ) const;

	QPoint alignToGrid( const QPoint& pos ) const;

	void setMode(Mode mode);
	
	OptimaElement * getLinkedElement(const QPointF & scenePos) const;

	///��������� ���� XML - ��������� ����� � ������� / �������� �� ����������� �����������
	template <class T>
	void loadElements(const QDomNodeList &elements, bool loadAllways);
	
	OptimaElement * getLinkedElement(const QString & elementUuid);

	QString getUuid(QGraphicsItem* item);

	///����� ������� �� ����� �� ��� uuid
	QGraphicsItem *findItem(const QString &findItemUuid);
	
	///����� ������� �� ����� �� ���������� ����������
	///������ �� ����� ������� � itemUuid �� xml, ���� ��� ���, �� ������� ���
	template <class T>
	T *getItem(const QString &itemUuid);

	void buildIntersectionConnectors();

	qreal findNextZOrder() const;

protected:

	virtual void keyPressEvent(QKeyEvent *event);


	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);


	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
	QDomDocument mPatternDoc;///<���� ������ ��������� ��� xml-��������� �������

	QDomDocument mDoc;///<���� ������ ��������� ��� xml-��������� 

	OptimaElement *mHoverElement;///<������ ��������� �� ������, ������� ���������� ��� ������������� ����������

	Mode mMode;///<������� �������� �������� ����� (������� ���������, ������, ���-������ �����������)
	
	qreal mAlignGridStep;///<��� ����������� �����

	OptimaConnectorBorderMarkerEnd* mNewConnectorEndMarker;///<������ �� vhrth �� ������� ����� ��� �������� ����������

	template <class T>
	void loadElement(const QDomNode &element, bool loadAllways);

	QDomNode getPatternNode(QString namePattern);

	virtual void apply();

	void updateHighlightElement(QPointF scenePos);

	QDomNode createConnectorXmlNode(OptimaConnectorMarker *marker);

	void addNewConnector(OptimaConnectorMarker *marker);

	void before1CCall();

	void loadWorkspace(const QDomNodeList &workspace);
};

template <class T>
void OptimaScene::loadElements(const QDomNodeList &elements, bool loadAllways)
{
	for ( int nn = 0; nn < elements.size( ); ++nn )
	{
		loadElement<T>(elements.at( nn ), loadAllways);	
	}
}

template <class T>
T * OptimaScene::getItem(const QString &itemUuid)
{
	const QList<QGraphicsItem*> itemList = items();
	for (QList<QGraphicsItem*>::const_iterator i = itemList.constBegin(); i != itemList.constEnd(); ++i )
	{
		QGraphicsItem* item = *i;
		QString uuid = getUuid(item);
		if (itemUuid == uuid)
		{
			return dynamic_cast<T*>(item);
		}
	}

	T *item = new T(itemUuid);
	addItem(item);			

	return item;
}