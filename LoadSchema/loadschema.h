#ifndef LOADSCHEMA_H
#define LOADSCHEMA_H

#include <QtGui/QMainWindow>
#include "ui_loadschema.h"

class LoadSchema : public QMainWindow
{
	Q_OBJECT

public:
	LoadSchema(QWidget *parent = 0, Qt::WFlags flags = 0);
	~LoadSchema();

private:
	Ui::LoadSchemaClass ui;
};

class OptimaView : public QGraphicsView
{
	Q_OBJECT;
}
#endif // LOADSCHEMA_H
