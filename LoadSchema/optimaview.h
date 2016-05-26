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
	void beforeExecute1CCall();
	void loadFigures(const QDomNodeList &figures, bool load_allways);
};
