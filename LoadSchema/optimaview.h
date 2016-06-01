#pragma once
#include <QtGui/QGraphicsView>
#include <QtXml/QDOMDocument>

class OptimaView : public QGraphicsView
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
	
	void beforeExecute1CCall();

	///Загружает файл XML - описателя сцены и создает / изменяет ее графическое отображение
	template <class T> void load(const QDomNodeList &elements, bool loadAllways);

	QGraphicsItem *findItem(const QString &itemUuid);
	
	///Найдем на схеме элемент с itemUuid из xml, если его нет, то зоздадим его
	template <class T>
	T *getItem(const QString &itemUuid);

	QString getUuid(QGraphicsItem* item);
};