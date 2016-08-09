#include "stdafx.h"
#include "tag.h"
#include "optimascene.h"
#include "optimaconnectorbordermarkerbegin.h"
#include "optimaconnectorbordermarkerend.h"
#include "OptimaConnectorMarker.h"
#include "optimaconnector.h"


OptimaConnectorMarker::OptimaConnectorMarker(OptimaConnector* parent, const OptimaPoint & pos, Qt::CursorShape cursorShape)
	: OptimaRectangleMarker(parent, pos, cursorShape, dynamic_cast<OptimaScene*>(parent->scene()))
	, mNewConnector(nullptr)
	, mCreatePos(pos)
	, mLinkedElement(nullptr)
	, mConnectorBeginMarker(nullptr)
	, mConnectorEndMarker(nullptr)
{
	setLinkedElement(nullptr);
}

OptimaConnectorMarker::OptimaConnectorMarker(OptimaScene* optimaScene, const OptimaPoint & pos, Qt::CursorShape cursorShape)
	: OptimaRectangleMarker(nullptr, pos, cursorShape, optimaScene)
	, mNewConnector(nullptr)
	, mCreatePos(pos)
	, mConnectorBeginMarker(nullptr)
	, mConnectorEndMarker(nullptr)
	, mLinkedElement(nullptr)
{
}

OptimaConnectorMarker::~OptimaConnectorMarker()
{
	if (mNewConnector != nullptr)
	{
		delete mNewConnector;
	}
	if (getLinkedElement() != nullptr)
	{
		getLinkedElement()->setLinkedHighlight(false);
	}
}

int OptimaConnectorMarker::type() const
{
	return Type;
}

void OptimaConnectorMarker::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	//необходимо для правильного расчета движения маркера
	OptimaRectangleMarker::mousePressEvent(event);

	OptimaConnector *connector = qgraphicsitem_cast<OptimaConnector*>(parentItem());
	
	Q_ASSERT(connector != nullptr);
	connector->setRebuild(true);
}

void OptimaConnectorMarker::setNewConnectorPoints()
{
	QPointF beginPoint( mConnectorBeginMarker->pos() );
	QPointF endPoint( mConnectorEndMarker->pos() );
	OptimaPointVector newPoints;

	OptimaFigure *beginFigure = dynamic_cast<OptimaFigure*>(mConnectorBeginMarker->getLinkedElement());
	OptimaFigure *endFigure = dynamic_cast<OptimaFigure*>(mConnectorEndMarker->getLinkedElement());
	OptimaConnector *beginConnector = dynamic_cast<OptimaConnector*>(mConnectorBeginMarker->getLinkedElement());
	OptimaConnector *endConnector = dynamic_cast<OptimaConnector*>(mConnectorEndMarker->getLinkedElement());

	//теперь можно узнать какую функцию надо вызвать для оперделения пути коннектора от
	//начальной к конечной точке
	switch( getRelationship() )
	{
	case OptimaTemporaryConnector::free_free:
		newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Free_Free( beginPoint, endPoint );
		break;
	case OptimaTemporaryConnector::free_figure:
		{
			Q_ASSERT(endFigure != nullptr);

			newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Free_Figure( beginPoint, endFigure->linkedRect());
		}
		break;
	case OptimaTemporaryConnector::free_connector:
		{
			Q_ASSERT(endConnector != nullptr);

			newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Free_Connector(beginPoint, endConnector->linkedLine(), endPoint);
		}
		break;
	case OptimaTemporaryConnector::connector_free:
		{
			Q_ASSERT(beginConnector != nullptr);

			newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Connector_Free(beginConnector->linkedLine(), beginPoint, endPoint );
		}
		break;
	case OptimaTemporaryConnector::connector_connector:
		{
			Q_ASSERT(beginConnector != nullptr);
			Q_ASSERT(endConnector != nullptr);

			newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Connector_Connector(beginConnector->linkedLine(), beginPoint, endConnector->linkedLine(), endPoint );
		}
		break;
	case OptimaTemporaryConnector::connector_figure:
		{
			Q_ASSERT(beginConnector != nullptr);
			Q_ASSERT(endFigure != nullptr);

			newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Connector_Figure(beginConnector->linkedLine(), beginPoint, endFigure->linkedRect());
		}
		break;
	case OptimaTemporaryConnector::figure_free:
		{
			Q_ASSERT(beginFigure != nullptr);

			newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Figure_Free( beginFigure->linkedRect(), endPoint );			
		}
		break;
	case OptimaTemporaryConnector::figure_figure:
		{
			Q_ASSERT(beginFigure != nullptr);
			Q_ASSERT(endFigure != nullptr);

			newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Figure_Figure( beginFigure->linkedRect(), endFigure->linkedRect() );
		}
		break;
	case OptimaTemporaryConnector::figure_connector:
		{
			Q_ASSERT(beginFigure != nullptr);
			Q_ASSERT(endConnector != nullptr);

			newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Figure_Connector( beginFigure->linkedRect(), endConnector->linkedLine(), endPoint );
		}				
		break;
	default:
		break;

	}

	mNewConnector->setPoints(newPoints);
}

void OptimaConnectorMarker::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	rebuildConnector();
}

void OptimaConnectorMarker::keyPressEvent(QKeyEvent *event)
{
	OptimaConnector *oldConnector = qgraphicsitem_cast<OptimaConnector*>(parentItem());
	if (mNewConnector == nullptr)
	{
		return;
	}
	
	switch(event->key())
	{
	case Qt::Key_Escape:
		delete mNewConnector;
		mNewConnector = nullptr;

		if ( oldConnector != nullptr)
		{
			oldConnector->setRebuild(false);
		}

		break;
	}
}

void OptimaConnectorMarker::updateLinkedHighlight()
{
	if (getLinkedElement() == nullptr)
		return;

	getLinkedElement()->setLinkedHighlight(true, mCreatePos);
}

QString OptimaConnectorMarker::getLinkedUuid(OptimaConnectorMarker *marker) const
{
	OptimaElement *linkedElement = marker->getLinkedElement();
	if (linkedElement == nullptr)
	{
		return "";
	}
	return linkedElement->uuid();
}

void OptimaConnectorMarker::rebuildConnector()
{
	OptimaConnector *oldConnector = qgraphicsitem_cast<OptimaConnector*>(parentItem());

	//укажем коннектору что его изменение завершилось
	oldConnector->setRebuild(false);

	//новый коннектор попрождается только при движении мыши, поэтому если его нет, 
	//то и сизменять нечего
	if ( mNewConnector == nullptr )
	{
		return;
	}

	oldConnector->setLinked( mConnectorBeginMarker->getLinkedElement(), mConnectorEndMarker->getLinkedElement());

	oldConnector->setPoints(getNewConnectorPoints());

	//buildIntersectionConnectors();

}

QString OptimaConnectorMarker::getLinkedBeginUuid() const
{
	return getLinkedUuid(mConnectorBeginMarker);
}

QString OptimaConnectorMarker::getLinkedEndUuid() const
{
	return getLinkedUuid(mConnectorEndMarker);
}

qreal OptimaConnectorMarker::getNewConnectorZValue() const
{
	Q_ASSERT(mNewConnector != nullptr);

	return mNewConnector->zValue();
}

bool OptimaConnectorMarker::isNewConnectorPresent() const
{
	return mNewConnector != nullptr;
}

int OptimaConnectorMarker::getLinkedType(OptimaConnectorMarker* connectorMarker) const
{
	const OptimaFigure *figure = dynamic_cast<const OptimaFigure*>(connectorMarker->getLinkedElement());
	const OptimaConnector *connector = dynamic_cast<const OptimaConnector*>(connectorMarker->getLinkedElement());

	if (figure != nullptr)
	{
		return tag::element::figure;
	} 
	else
		if (connector != nullptr)
		{
			return tag::element::connector;
		}
		else
		{
			return tag::element::free;			
		}
}

OptimaTemporaryConnector::Relationship OptimaConnectorMarker::getRelationship()
{
	//ссылки на начальный и конечный маркер есть даже у линейных маркеров
	Q_ASSERT(mConnectorBeginMarker != nullptr);
	Q_ASSERT(mConnectorEndMarker != nullptr);

	int  relationship = getLinkedType(mConnectorBeginMarker) << 4 | getLinkedType(mConnectorEndMarker);

	return OptimaTemporaryConnector::Relationship(relationship); 
}

void OptimaConnectorMarker::setLinkedElement(OptimaElement* linkedElement)
{
	mLinkedElement = linkedElement;
	setLinked(mLinkedElement != nullptr);
}

void OptimaConnectorMarker::setLinked(bool val)
{
	mLinked = val;

	QPen pen( val ? tag::markerLinkColor : tag::markerFreeColor, 1.0 );
	pen.setCosmetic( true );
	setPen( pen );			
}

void OptimaConnectorMarker::setMarkerPos(const QPointF & scenePos)
{
	if (getLinkedElement() != nullptr)
	{
		getLinkedElement()->setLinkedHighlight(false);
	}

	setLinkedElement(mOptimaScene->getLinkedElement(scenePos));
	setPos(scenePos);

	if (getLinkedElement() != nullptr)
	{
		getLinkedElement()->setLinkedHighlight(true, scenePos);
	}

	//обновим выделение у того маркера, который не ведем мышью
	if (mConnectorBeginMarker != this)
	{
		mConnectorBeginMarker->updateLinkedHighlight();
	}
	if (mConnectorEndMarker != this)
	{
		mConnectorEndMarker->updateLinkedHighlight();
	}

	//начальный маркер может существовать без конечного, но конечный без начального не может
	//эта проверка покажет что у нас есть оба маркера и мы можем строить между нимим коннектор
	if (mConnectorEndMarker != nullptr)
	{
		//временного коннектора еще нет, создадим его, именно он будет показывать, как измениться существующий 
		//или создастся новый коннектор
		if (mNewConnector == nullptr)
		{
			OptimaConnector *connector = qgraphicsitem_cast<OptimaConnector*>(parentItem());

			//
			if (connector == nullptr)
			{
				//создаем на сцене новый коннектор
				mNewConnector = new OptimaTemporaryConnector(scene(), OptimaPoint::createVector(mConnectorBeginMarker->pos(), mConnectorEndMarker->pos()));	
			} 
			else
			{
				//изменяем существующий коннктор
				mNewConnector = new OptimaTemporaryConnector(connector);	
			}
			
			
			mNewConnector->setZValue(mOptimaScene->findNextZOrder());
		}

		setNewConnectorPoints();
	}
}

OptimaPointVector OptimaConnectorMarker::getNewConnectorPoints()
{
	Q_ASSERT(mNewConnector != nullptr);

	OptimaPointVector points(mNewConnector->points());

	OptimaElement *startElement  = mConnectorBeginMarker->getLinkedElement();
	OptimaElement *endElement  = mConnectorEndMarker->getLinkedElement();

	//если передвигаем начало коннеткора, а не конец
	if (startElement != nullptr)
	{
		points.first() = startElement->getIntersectPoint(QLineF(points.at(0), points.at(1)));
	}
	if (endElement != nullptr)
	{
		points.last() = endElement->getIntersectPoint(QLineF(points.at(points.size()-1), points.at(points.size()-2)));
	}

	return points;
}
