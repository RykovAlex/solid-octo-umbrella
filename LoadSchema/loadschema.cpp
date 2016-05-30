#include "stdafx.h"
#include "loadschema.h"

LoadSchema::LoadSchema(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	ui.action->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
}

LoadSchema::~LoadSchema()
{

}

void LoadSchema::loadXml()
{
	QFileDialog::Options options;
	options |=QFileDialog::DontUseNativeDialog;
	QString selectedFilter;

	QString fileName =QFileDialog::getOpenFileName(this,
		tr("QFileDialog::getOpenFileName()"),
		tr(""),
		tr("XML files (*.xml)"),&selectedFilter,
		options);
	
	if (fileName.isEmpty())
		return;
	
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly |QIODevice::Text))
		return;

	QTextStream in(&file);

	QString line = in.readAll();
	ui.graphicsView->LoadScheme(line, true);

}
