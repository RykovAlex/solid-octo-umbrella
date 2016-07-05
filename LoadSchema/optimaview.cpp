#include "stdafx.h"
#include "tag.h"
#include "optimaconnector.h"
#include "optimaview.h"
#include "optimatext.h"
#include "optimaconnectorpathfinder.h"
#include "optimatemporaryconnector.h"

OptimaView::OptimaView(QWidget *parent) 
	: QGraphicsView(parent)
	, mMode(MoveItem)
	, newConnector(nullptr)
	, linkedElement(nullptr)
	, linkedStartElement(nullptr)
{ 
	setScene( new QGraphicsScene(parent) );
	//scene()->setSceneRect(QRectF(0.0, 0.0, 3000.0,3000.0));
	setMouseTracking(true);
};

void OptimaView::setMode(Mode mode)
{
	mMode = mode;
}

void OptimaView::setLinkedHighlight(bool enabled, const QPointF & scenePos /*= QPointF() */)
{
	throw std::logic_error("The method or operation is not implemented.");
}

bool OptimaView::checkLinkedHighlight(const QPointF & scenePos)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void OptimaView::addConnector(QMouseEvent * mouseEvent)
{
	Q_ASSERT (newConnector == nullptr);
	
	QPointF scenePos = mapToScene(mouseEvent->pos());

	newConnector = new OptimaTemporaryConnector(scene(), OptimaPoint::createVector(scenePos, scenePos));	
	
	//scene()->addItem(newConnector);

	//newConnector = new OptimaTemporaryConnector();
	//newConnector->setPoints(OptimaPoint::createVector(scenePos, scenePos));
}

void OptimaView::mousePressEvent(QMouseEvent *mouseEvent)
{
	if (mouseEvent->button() != Qt::LeftButton)
		return;

	switch(mMode)
	{
	case InsertItem:
		break;
	case InsertLine:
		addConnector(mouseEvent);
		return;
	case InsertText:
		break;
	case MoveItem:
		break;
	default:
		break;
	}
	QGraphicsView::mousePressEvent(mouseEvent);
}

void OptimaView::mouseMoveEvent(QMouseEvent *mouseEvent)
{
	QPoint eventPos = mouseEvent->pos();

	QPointF scenePos = mapToScene(eventPos);

	//подсветка областей к которым будем коннектиться
	if (mMode == InsertLine)
	{
		updateHighlightLinkedElement(scenePos);
		updateHighlightStartLinkedElement();	

		if (newConnector != nullptr )
		{
			Q_ASSERT(newConnector->childItems().count() >= 2);

			//известим коннектор о передвижении его финальной точки
			newConnector->onEndBorderMove(scenePos);
			
			OptimaPointVector newPoints;

			//теперь можно узнать какую функцию надо вызвать для оперделения пути коннектора от
			//начальной к конечной точке
			switch(newConnector->getRelationship())
			{
			case OptimaTemporaryConnector::free_free:
				newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Free_Free( newConnector->first(), scenePos );
				break;
			case OptimaTemporaryConnector::free_figure:
				{
					OptimaFigure *endFigure = dynamic_cast<OptimaFigure*>(getLinkedElement(scenePos));
					
					newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Free_Figure( newConnector->first(), endFigure->linkedRect());
				}
				break;
			case OptimaTemporaryConnector::free_connector:
				{
					OptimaConnector *endConnector = dynamic_cast<OptimaConnector*>(getLinkedElement(scenePos));

					newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Free_Connector(newConnector->first(), endConnector->linkedLine(), scenePos);
				}
				break;
			case OptimaTemporaryConnector::connector_free:
				{
					OptimaConnector *beginConnector = dynamic_cast<OptimaConnector*>(getLinkedElement(newConnector->first()));

					newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Connector_Free(beginConnector->linkedLine(), newConnector->first(), scenePos );
				}
				break;
			case OptimaTemporaryConnector::connector_connector:
				{
					OptimaConnector *beginConnector = dynamic_cast<OptimaConnector*>(getLinkedElement(newConnector->first()));
					OptimaConnector *endConnector = dynamic_cast<OptimaConnector*>(getLinkedElement(scenePos));

					newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Connector_Connector(beginConnector->linkedLine(), newConnector->first(), endConnector->linkedLine(), scenePos );
				}
				break;
			case OptimaTemporaryConnector::connector_figure:
				{
					OptimaConnector *beginConnector = dynamic_cast<OptimaConnector*>(getLinkedElement(newConnector->first()));
					OptimaFigure *endFigure = dynamic_cast<OptimaFigure*>(getLinkedElement(scenePos));

					newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Connector_Figure(beginConnector->linkedLine(), newConnector->first(), endFigure->linkedRect());
				}
				break;
			case OptimaTemporaryConnector::figure_free:
				{
					OptimaFigure *beginFigure = dynamic_cast<OptimaFigure*>(getLinkedElement(newConnector->first()));

					newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Figure_Free( beginFigure->linkedRect(), scenePos );
					
					if (newPoints.first() != newConnector->points().first())
					{
						newConnector->onBeginBorderMove(newPoints.first());
					}
					
				}
				break;
			case OptimaTemporaryConnector::figure_figure:
				{
					OptimaFigure *beginFigure = dynamic_cast<OptimaFigure*>(getLinkedElement(newConnector->first()));
					OptimaFigure *endFigure = dynamic_cast<OptimaFigure*>(getLinkedElement(scenePos));

					newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Figure_Figure( beginFigure->linkedRect(), endFigure->linkedRect() );

					if (newPoints.first() != newConnector->points().first())
					{
						newConnector->onBeginBorderMove(newPoints.first());
					}

				}
				break;
			case OptimaTemporaryConnector::figure_connector:
				{
					OptimaFigure *beginFigure = dynamic_cast<OptimaFigure*>(getLinkedElement(newConnector->first()));
					OptimaConnector *endConnector = dynamic_cast<OptimaConnector*>(getLinkedElement(scenePos));

					newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Figure_Connector( beginFigure->linkedRect(), endConnector->linkedLine(), scenePos );
				}				
				break;
			default:
				break;

			}

			newConnector->setPoints(newPoints);

		} 
	}
	else
	{
		QGraphicsView::mouseMoveEvent(mouseEvent);
	}

}

void OptimaView::setMarkerPen(QGraphicsRectItem * borderEnd, QPointF scenePos)
{

}

void OptimaView::updateHighlightLinkedElement(QPointF scenePos)
{
	if (linkedElement != nullptr && linkedElement != linkedStartElement )
	{
		linkedElement->setLinkedHighlight(false);
	}

	linkedElement = getLinkedElement(scenePos);

	if (linkedElement != nullptr && linkedElement != linkedStartElement)
	{
		linkedElement->setLinkedHighlight(true, scenePos);
	}
}

void OptimaView::updateHighlightStartLinkedElement()
{
	if (linkedStartElement != nullptr)
	{
		linkedStartElement->setLinkedHighlight(false);
		linkedStartElement = nullptr;
	}

	if (newConnector != nullptr)
	{
		Q_ASSERT(newConnector->points().count() >= 2);

		QPointF scenePos = newConnector->startPoint();

		linkedStartElement = getLinkedElement(scenePos);

		if (linkedStartElement != nullptr)
		{
			linkedStartElement->setLinkedHighlight(true, scenePos);
		}
	}
}

void OptimaView::keyPressEvent(QKeyEvent *keyEvent)
{
	if (newConnector != nullptr)
	{
		switch(keyEvent->key())
		{
		case Qt::Key_Escape:
			//QCursor::setPos(QPoint(1300,1300));

			delete newConnector;
			newConnector = nullptr;
			
			break;
		}
	}
}

void OptimaView::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
	if (newConnector != nullptr)
	{
		if (newConnector->first() != newConnector->last())
		{
			scene()->addItem(new OptimaConnector(newConnector, this));	
		}

		delete newConnector;
		newConnector = nullptr;

		updateHighlightStartLinkedElement();	
	}
	QGraphicsView::mouseReleaseEvent(mouseEvent);
}

void OptimaView::apply()
{
	// переменные которые не будут изменяться пользователем
	mAlignGridStep = getXmlValue(tag::virtual_step, 4.0);
}

void OptimaView::draw(bool isProcessLoading /*= false*/)
{
	//throw std::logic_error("The method or operation is not implemented.");
}

template <class T>
void OptimaView::loadElements(const QDomNodeList &elements, bool loadAllways)
{
	for ( int nn = 0; nn < elements.size( ); ++nn )
	{
		const QDomNode &element = elements.at( nn );
		const QString uuid = element.namedItem( tag::id ).toElement().text();

		if (uuid.isEmpty())
		{			
			if ( !loadAllways )
				throw QString().append( tr( "У элемента не найден UUID" ) );
			continue;
		}
		
		//Найдем на схеме элемент с itemUuid из xml, если его нет, то создадим его
		T *item = getItem<T>(uuid);

		Q_ASSERT( nullptr != item );

		//Запомним переданный или изменим текущий xml элемента и применим результирующий xml 
		//к графическому элементу, после этого он отрисуется на схеме
		item->applyXml(element);
		
		item->draw(true);
	}
}

QGraphicsItem *OptimaView::findItem(const QString &itemUuid)
{
	const QList<QGraphicsItem*> itemList = items();
	for (QList<QGraphicsItem*>::const_iterator i = itemList.constBegin(); i != itemList.constEnd(); ++i )
	{
		QGraphicsItem* item = *i;
		QString itemUuid = getUuid(item);
		if (itemUuid == itemUuid)
		{
			return item;
		}
	}

	return nullptr;
}

template <class T>
T *OptimaView::getItem(const QString &itemUuid)
{
	const QList<QGraphicsItem*> itemList = items();
	for (QList<QGraphicsItem*>::const_iterator i = itemList.constBegin(); i != itemList.constEnd(); ++i )
	{
		QGraphicsItem* item = *i;
		QString uuid = getUuid(item);
		if (itemUuid == uuid)
		{
			return dynamic_cast<T*>(item);
		}
	}
	
	T *item = new T(itemUuid, this);
	scene()->addItem(item);			
	
	return item; 
}

QString OptimaView::getUuid(QGraphicsItem* item)
{
	return item->data(tag::data::uuid).toString();
}


void OptimaView::buildIntersectionConnectors()
{
	const QList<QGraphicsItem*> itemList = items();

	//очистку приходится делать отдельно, так как пересечения с углами не подчиняется правилу zOrder
	for (QList<QGraphicsItem*>::const_iterator i = itemList.constBegin(); i != itemList.constEnd(); ++i )
	{
		OptimaConnector* item = dynamic_cast<OptimaConnector*>(*i);
		if (item == nullptr)
		{
			continue;
		}

		//очистим предыдущие точки пересечения
		item->clearIntersection();
	}

	for (QList<QGraphicsItem*>::const_iterator i = itemList.constBegin(); i != itemList.constEnd(); ++i )
	{
		OptimaConnector* item = dynamic_cast<OptimaConnector*>(*i);
		if (item == nullptr)
		{
			continue;
		}
		
		item->getIntersection(itemList, i - itemList.constBegin());

		item->draw();
	}

}

void OptimaView::loadWorkspace(const QDomNodeList &workspace)
{
	for ( int nn = 0; nn < workspace.size( ); ++nn )
	{
		const QDomNode &element = workspace.at( nn );

		//Запомним переданный или изменим текущий xml элемента и применим результирующий xml 
		//к графическому элементу, после этого он отрисуется на схеме
		applyXml(element);

		draw(true);
	}

}

QString OptimaView::LoadScheme(const QString &filename, bool load_allways)
{
	beforeExecute1CCall();
	
	try
	{
		int xml_error_column;
		int xml_error_line;
		QString xml_error_msg;

		if ( !doc.setContent( filename, &xml_error_msg, &xml_error_line, &xml_error_column ) )
			throw QString().append( tr( "Ошибка при анализе xml, сообщение: %1, строка: %2, столбец: %3\n" ).arg( xml_error_msg ).arg( xml_error_line ).arg( xml_error_column ) );

		const QDomElement docElement = doc.documentElement( );
		//// определяем цвет бек-граунда
		//const QDomNode dn_no_workspace = doc_el.namedItem( tag::no_workspace );
		//if ( !dn_no_workspace.isNull( ) )
		//{
		//	m_workspace.m_no_back_color = convert_to_color( dn_no_workspace, tag::back_color, m_workspace.m_no_back_color );
		//	const QDomNode dn_up = dn_no_workspace.namedItem( tag::user_properties );
		//	if ( !dn_up.isNull( ) )
		//	{
		//		m_workspace.m_no_user_property = dn_up.toElement( ).text( );
		//	}
		//}
		//Загружаем рабочее пространство
		loadWorkspace(docElement.elementsByTagName( tag::workspace ));
		
		//Загружаем элементы схемы
		loadElements<OptimaFigure>( docElement.elementsByTagName( tag::figure ), load_allways );
		loadElements<OptimaConnector>( docElement.elementsByTagName( tag::line ), load_allways );
		loadElements<OptimaText>( docElement.elementsByTagName( tag::text_label ), load_allways );

		buildIntersectionConnectors();
	}
	catch (std::exception* e)
	{
		Q_EMIT ErrorOccur( tr("LoadScheme: %1").arg(e->what()) );
	}
	catch (const QString &errorString)
	{
		return errorString;
	}
	catch ( ... )
	{
		Q_EMIT ErrorOccur( tr("LoadScheme: Неизвестная ошибка") );
	}		

	return "";

}

void OptimaView::beforeExecute1CCall()
{

}
