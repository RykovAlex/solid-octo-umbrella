#pragma once
#include "optimarectanglemarker.h"
#include "optimatemporaryconnector.h"

class OptimaConnector;
class OptimaConnectorBorderMarkerBegin;
class OptimaConnectorBorderMarkerEnd;
class OptimaElement;
class OptimaConnectorMarker : public OptimaRectangleMarker
{
	Q_DISABLE_COPY( OptimaConnectorMarker );

public:
	enum { Type = UserType + tag::element::borderMarker };

	OptimaConnectorMarker(OptimaConnector* parent, const OptimaPoint & pos, Qt::CursorShape cursorShape, int сhangeIndex);

	OptimaConnectorMarker(OptimaScene* optimaScene, const OptimaPoint & pos, Qt::CursorShape cursorShape);

	~OptimaConnectorMarker();


	virtual int type() const;

	virtual void setMarkerPos(const QPointF & pos);

	OptimaPointVector getNewConnectorPoints();

	void updateLinkedHighlight();
	
	QString getLinkedBeginUuid() const;

	QString getLinkedEndUuid() const;
	
	qreal getNewConnectorZValue() const;

	bool isNewConnectorPresent() const;

	inline OptimaElement *getLinkedElement() const { return mLinkedElement;  }

protected:
	OptimaTemporaryConnector *mNewConnector;///<отображение для коннетора который только еще создается

	OptimaConnectorBorderMarkerBegin* mConnectorBeginMarker;///<ссылка на маркер в начале коннектора, для самого коннектора начала это будет this 

	OptimaConnectorBorderMarkerEnd* mConnectorEndMarker;///<ссылка на маркер в конце коннектора, для самого коннектора конца это будет this 

	OptimaElement *mLinkedElement;///<ссылка на элемент к кторому приуреплен маркер

	int getLinkedType(OptimaConnectorMarker* connectorMarker) const;

	void setLinkedElement(OptimaElement* linkedElement);

	void setLinked(bool val);
	
	bool linked() const { return mLinked; }

	QPointF mCreatePos;

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

	void setNewConnectorPoints();

	///получить код положения маркера относительно других элементов
	OptimaTemporaryConnector::Relationship getRelationship();

	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);


	virtual void keyPressEvent(QKeyEvent *event);

private:
	bool mLinked; ///< признак того, что этот маркер отображает присоединение
	
	int mChangeIndex; ///< некий индекс изменяемого элемента точка\или линия

	QString getLinkedUuid(OptimaConnectorMarker *marker) const;

	void rebuildConnector();


};


