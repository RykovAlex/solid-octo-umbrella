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

	OptimaConnectorMarker(OptimaConnector* parent, const OptimaPoint & pos, Qt::CursorShape cursorShape, int �hangeIndex);

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
	OptimaTemporaryConnector *mNewConnector;///<����������� ��� ��������� ������� ������ ��� ���������

	OptimaConnectorBorderMarkerBegin* mConnectorBeginMarker;///<������ �� ������ � ������ ����������, ��� ������ ���������� ������ ��� ����� this 

	OptimaConnectorBorderMarkerEnd* mConnectorEndMarker;///<������ �� ������ � ����� ����������, ��� ������ ���������� ����� ��� ����� this 

	OptimaElement *mLinkedElement;///<������ �� ������� � ������� ���������� ������

	int getLinkedType(OptimaConnectorMarker* connectorMarker) const;

	void setLinkedElement(OptimaElement* linkedElement);

	void setLinked(bool val);
	
	bool linked() const { return mLinked; }

	QPointF mCreatePos;

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

	void setNewConnectorPoints();

	///�������� ��� ��������� ������� ������������ ������ ���������
	OptimaTemporaryConnector::Relationship getRelationship();

	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);


	virtual void keyPressEvent(QKeyEvent *event);

private:
	bool mLinked; ///< ������� ����, ��� ���� ������ ���������� �������������
	
	int mChangeIndex; ///< ����� ������ ����������� �������� �����\��� �����

	QString getLinkedUuid(OptimaConnectorMarker *marker) const;

	void rebuildConnector();


};


