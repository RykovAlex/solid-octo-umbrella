#include "stdafx.h"
#include "loadschema.h"
#include "optimascene.h"

LoadSchema::LoadSchema(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	ui.action->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
	ui.action_2->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
	
	
	redoAction = ui.graphicsView->undoStack->createRedoAction(this);
	undoAction = ui.graphicsView->undoStack->createUndoAction(this);
	

	undoAction->setFont(ui.action->font());
	
	ui.mainToolBar->addAction(undoAction);
	ui.mainToolBar->addAction(redoAction);


	QToolButton *pointerButton = new QToolButton;
	pointerButton->setCheckable(true);
	pointerButton->setChecked(true);
	pointerButton->setIcon(QIcon(":/images/resources/pointer.png"));
	QToolButton *linePointerButton = new QToolButton;
	linePointerButton->setCheckable(true);
	linePointerButton->setIcon(QIcon(":/images/resources/linepointer.png"));

	pointerTypeGroup = new QButtonGroup(this);
	pointerTypeGroup->addButton(pointerButton, int(OptimaScene::MoveItem));
	pointerTypeGroup->addButton(linePointerButton,
		int(OptimaScene::InsertLine));
	connect(pointerTypeGroup, SIGNAL(buttonClicked(int)),
		this, SLOT(pointerGroupClicked(int)));

	pointerToolbar = addToolBar(tr("Pointer type"));
	pointerToolbar->addWidget(pointerButton);
	pointerToolbar->addWidget(linePointerButton);

	createUndoView();
}

LoadSchema::~LoadSchema()
{

}

void LoadSchema::createUndoView()
{
	undoView = new QUndoView(ui.graphicsView->undoStack);
	undoView->setWindowTitle(tr("Command List"));
	//undoView->setLocale(ui.mainToolBar->locale());
	undoView->show();
	undoView->setAttribute(Qt::WA_QuitOnClose, false);
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
	QString linePattern;

	QFile filePattern(fileName + QString(".pattern"));
	if (filePattern.open(QIODevice::ReadOnly |QIODevice::Text))
	{
		QTextStream inPattern(&filePattern);
		linePattern = inPattern.readAll();
	}


	ui.graphicsView->LoadScheme(line, linePattern, true);

}

void LoadSchema::saveXml()
{
	QFileDialog::Options options;
	options |=QFileDialog::DontUseNativeDialog;
	QString selectedFilter;

	QString fileName =QFileDialog::getSaveFileName(this,
		tr("QFileDialog::getSaveFileName()"),
		tr(""),
		tr("XML files (*.xml)"),&selectedFilter,
		options);

	if (fileName.isEmpty())
		return;

	QString line;

	ui.graphicsView->SaveScheme(line);

	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
		return;

	QTextStream out(&file);

	out << line;

}

void LoadSchema::pointerGroupClicked(int id)
{
	ui.graphicsView->setMode(OptimaScene::Mode(pointerTypeGroup->checkedId()));
}
