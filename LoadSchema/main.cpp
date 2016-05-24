#include "stdafx.h"
#include "loadschema.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	LoadSchema w;
	w.show();
	return a.exec();
}
