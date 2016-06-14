#pragma once
#include <QtGui/QGraphicsView>
#include <QtXml/QDOMDocument>
#include "OptimaElement.h"

class OptimaView : public QGraphicsView, public OptimaElement
{
	Q_OBJECT;
public:
	OptimaView(QWidget *parent = 0);


public Q_SLOTS:
	QString LoadScheme(const QString &filename, bool load_allways);


Q_SIGNALS:
	void ErrorOccur( const QString & text );

private:
	QDomDocument doc;	
	
	virtual void apply();

	virtual void draw(bool isProcessLoading = false);

	void beforeExecute1CCall();

	///��������� ���� XML - ��������� ����� � ������� / �������� �� ����������� �����������
	template <class T> void loadElements(const QDomNodeList &elements, bool loadAllways);

	QGraphicsItem *findItem(const QString &itemUuid);
	
	///������ �� ����� ������� � itemUuid �� xml, ���� ��� ���, �� �������� ���
	template <class T>
	T *getItem(const QString &itemUuid);

	QString getUuid(QGraphicsItem* item);
	
	void buildIntersectionConnectors();
	
	void loadWorkspace(const QDomNodeList &workspace);
};