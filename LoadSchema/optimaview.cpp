#include "stdafx.h"
#include "tag.h"
#include "optimaconnector.h"
#include "optimaview.h"
#include "optimatext.h"
#include "optimaconnectorpathfinder.h"
#include "optimatemporaryconnector.h"
#include "optimaconnectorbordermarkerbegin.h"
#include "optimaconnectorbordermarkerend.h"

class OptimaConnectorMarker;

QString OptimaView::LoadScheme(const QString &xml, const QString &xmlPattern, bool load_allways)
{
	return scene()->LoadScheme(xml, xmlPattern, load_allways);
}

OptimaView::OptimaView(QWidget *parent) 
	: QGraphicsView(parent)
{ 
	undoStack = new QUndoStack(this);

	OptimaScene *optimaScene = new OptimaScene( undoStack );
	setScene( optimaScene );
	//scene()->setSceneRect(QRectF(0.0, 0.0, 3000.0,3000.0));
	setMouseTracking(false);
};

OptimaScene *OptimaView::scene()
{
	OptimaScene * optimaScene = dynamic_cast<OptimaScene *>(QGraphicsView::scene());
	Q_ASSERT (NULL != optimaScene);

	return optimaScene;
}

void OptimaView::setMode(OptimaScene::Mode mode)
{
	scene()->setMode(mode);
}

void OptimaView::SaveScheme(QString &line)
{
	line = scene()->SaveScheme();
}

