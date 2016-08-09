#pragma once
#include <QtGui/QGraphicsView>
#include <QtXml/QDOMDocument>
#include "OptimaElement.h"
#include "optimafigure.h"
#include "optimatemporaryconnector.h"
#include "optimaconnectorpathfinder.h"
#include "optimascene.h"

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
	OptimaView(QWidget *parent = 0);

	OptimaScene *scene();

	void setMode(OptimaScene::Mode mode);

protected:

private:
	
};
