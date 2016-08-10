#pragma once
#include <QtGui/QGraphicsView>
#include <QtXml/QDOMDocument>
#include "OptimaElement.h"
#include "optimafigure.h"
#include "optimatemporaryconnector.h"
#include "optimaconnectorpathfinder.h"
#include "optimascene.h"
#include "../../Qt/4.8.7/include/QtGui/QUndoStack"

class OptimaConnector;
class OptimaTemporaryConnector;
class OptimaConnectorMarker;
class OptimaConnectorBorderMarkerBegin;
class OptimaConnectorBorderMarkerEnd;
class OptimaScene;
class OptimaView : public QGraphicsView
{
	Q_OBJECT;

public Q_SLOTS:
	QString LoadScheme(const QString &xml, const QString &xmlPattern, bool load_allways);

Q_SIGNALS:
	void ErrorOccur( const QString & text );

public:	
	QUndoStack *undoStack;

	OptimaView(QWidget *parent = 0);

	OptimaScene *scene();

	void setMode(OptimaScene::Mode mode);

	void SaveScheme(QString &line);

protected:

private:

		 
};
