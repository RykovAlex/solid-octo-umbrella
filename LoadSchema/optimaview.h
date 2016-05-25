#pragma once
#include <QtGui/QGraphicsView>

class OptimaView : public QGraphicsView
{
	Q_OBJECT;
public:
	OptimaView(QWidget *parent = 0) : QGraphicsView(parent) {};
};


