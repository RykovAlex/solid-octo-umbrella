#include "stdafx.h"
#include "loadschema.h"

LoadSchema::LoadSchema(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	ui.action->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));

	QToolButton *pointerButton = new QToolButton;
	pointerButton->setCheckable(true);
	pointerButton->setChecked(true);
	pointerButton->setIcon(QIcon(":/images/resources/pointer.png"));
	QToolButton *linePointerButton = new QToolButton;
	linePointerButton->setCheckable(true);
	linePointerButton->setIcon(QIcon(":/images/resources/linepointer.png"));

	pointerTypeGroup = new QButtonGroup(this);
	pointerTypeGroup->addButton(pointerButton, int(OptimaView::MoveItem));
	pointerTypeGroup->addButton(linePointerButton,
		int(OptimaView::InsertLine));
	connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
		this, SLOT(pointerGroupClicked(int)));

	pointerToolbar = addToolBar(tr("Pointer type"));
	pointerToolbar->addWidget(pointerButton);
	pointerToolbar->addWidget(linePointerButton);

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

void LoadSchema::pointerGroupClicked(int id)
{
	ui.graphicsView->setMode(OptimaView::Mode(pointerTypeGroup->checkedId()));
}
