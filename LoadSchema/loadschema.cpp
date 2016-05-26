#include "stdafx.h"
#include "loadschema.h"

LoadSchema::LoadSchema(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	//ui.action->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
}

LoadSchema::~LoadSchema()
{

}

void LoadSchema::loadXml()
{

}
