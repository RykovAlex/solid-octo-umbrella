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
	, mNewConnector(nullptr)
	, mOldConnector(nullptr)
	, mLinkedBeginElement(nullptr)
	, mLinkedEndElement(nullptr)
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
	Q_ASSERT (mNewConnector == nullptr);
	
	QPointF scenePos = mapToScene(mouseEvent->pos());

	mNewConnector = new OptimaTemporaryConnector(scene(), OptimaPoint::createVector(scenePos, scenePos));	
	mNewConnector->setZValue(findNextZOrder());
}

void OptimaView::addConnector(OptimaConnector * oldConnector, bool reversed)
{
	Q_ASSERT (mNewConnector == nullptr);

	mMode = InsertLine;
	mOldConnector = oldConnector;

	OptimaPointVector pointVector;
	OptimaConnectorArrowVector connectorArrowVector;
	OptimaPointVector connectorPoints(oldConnector->points());

	pointVector << connectorPoints.first() << connectorPoints.last();
	connectorArrowVector << oldConnector->beginArrow() << oldConnector->endArrow();

	mNewConnector = new OptimaTemporaryConnector(scene(), pointVector, connectorArrowVector, reversed);	
}

QPointF OptimaView::getIntersectPoint(const QLineF line) const
{
	return QPointF();
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

		if (mNewConnector != nullptr )
		{
			Q_ASSERT(mNewConnector->childItems().count() >= 2);

			QPointF beginPoint;
			QPointF endPoint;
			
			//известим коннектор о передвижении его крайнего коннетора
			mNewConnector->onEndBorderMove(scenePos, beginPoint, endPoint);
			
			OptimaPointVector newPoints;

			//теперь можно узнать какую функцию надо вызвать для оперделения пути коннектора от
			//начальной к конечной точке
			switch(mNewConnector->getRelationship())
			{
			case OptimaTemporaryConnector::free_free:
				newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Free_Free( beginPoint, endPoint );
				break;
			case OptimaTemporaryConnector::free_figure:
				{
					OptimaFigure *endFigure = dynamic_cast<OptimaFigure*>(getLinkedElement(endPoint));
					
					newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Free_Figure( beginPoint, endFigure->linkedRect());
				}
				break;
			case OptimaTemporaryConnector::free_connector:
				{
					OptimaConnector *endConnector = dynamic_cast<OptimaConnector*>(getLinkedElement(endPoint));

					newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Free_Connector(beginPoint, endConnector->linkedLine(), endPoint);
				}
				break;
			case OptimaTemporaryConnector::connector_free:
				{
					OptimaConnector *beginConnector = dynamic_cast<OptimaConnector*>(getLinkedElement(beginPoint));
					
					qDebug()<< "connector_free" << endPoint;
					newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Connector_Free(beginConnector->linkedLine(), beginPoint, endPoint );
				}
				break;
			case OptimaTemporaryConnector::connector_connector:
				{
					OptimaConnector *beginConnector = dynamic_cast<OptimaConnector*>(getLinkedElement(beginPoint));
					OptimaConnector *endConnector = dynamic_cast<OptimaConnector*>(getLinkedElement(endPoint));
					
					qDebug()<< "connector_connector" << endPoint;
					newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Connector_Connector(beginConnector->linkedLine(), beginPoint, endConnector->linkedLine(), endPoint );
				}
				break;
			case OptimaTemporaryConnector::connector_figure:
				{
					OptimaConnector *beginConnector = dynamic_cast<OptimaConnector*>(getLinkedElement(beginPoint));
					OptimaFigure *endFigure = dynamic_cast<OptimaFigure*>(getLinkedElement(endPoint));

					newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Connector_Figure(beginConnector->linkedLine(), beginPoint, endFigure->linkedRect());
				}
				break;
			case OptimaTemporaryConnector::figure_free:
				{
					OptimaFigure *beginFigure = dynamic_cast<OptimaFigure*>(getLinkedElement(beginPoint));

					newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Figure_Free( beginFigure->linkedRect(), endPoint );
					
					//если ведем не конечный, а начальный маркер фигуры
					if (beginPoint == scenePos)
					{
						if (newPoints.last() != mNewConnector->last())
						{
							mNewConnector->onBeginBorderMove(newPoints.last());
						}
					}
					else
					{
						if (newPoints.first() != mNewConnector->first())
						{
							mNewConnector->onBeginBorderMove(newPoints.first());
						}

					}			
				}
				break;
			case OptimaTemporaryConnector::figure_figure:
				{
					OptimaFigure *beginFigure = dynamic_cast<OptimaFigure*>(getLinkedElement(beginPoint));
					OptimaFigure *endFigure = dynamic_cast<OptimaFigure*>(getLinkedElement(endPoint));

					newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Figure_Figure( beginFigure->linkedRect(), endFigure->linkedRect() );

					if (beginPoint == scenePos)
					{
						if (newPoints.last() != mNewConnector->last())
						{
							mNewConnector->onBeginBorderMove(newPoints.last());
						}
					}
					else
					{
						if (newPoints.first() != mNewConnector->first())
						{
							mNewConnector->onBeginBorderMove(newPoints.first());
						}

					}
				}
				break;
			case OptimaTemporaryConnector::figure_connector:
				{
					OptimaFigure *beginFigure = dynamic_cast<OptimaFigure*>(getLinkedElement(beginPoint));
					OptimaConnector *endConnector = dynamic_cast<OptimaConnector*>(getLinkedElement(endPoint));

					newPoints = OptimaConnectorPathFinder::GetNewConnectorPoints_Figure_Connector( beginFigure->linkedRect(), endConnector->linkedLine(), endPoint );
				}				
				break;
			default:
				break;

			}

			mNewConnector->setPoints(newPoints);

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
	if (mLinkedEndElement != nullptr && mLinkedEndElement != mLinkedBeginElement )
	{
		mLinkedEndElement->setLinkedHighlight(false);
	}

	mLinkedEndElement = getLinkedElement(scenePos);

	if (mLinkedEndElement != nullptr && mLinkedEndElement != mLinkedBeginElement)
	{
		mLinkedEndElement->setLinkedHighlight(true, scenePos);
	}
}

void OptimaView::updateHighlightStartLinkedElement()
{
	if (mLinkedBeginElement != nullptr)
	{
		mLinkedBeginElement->setLinkedHighlight(false);
		mLinkedBeginElement = nullptr;
	}

	if (mNewConnector != nullptr)
	{
		Q_ASSERT(mNewConnector->points().count() >= 2);

		QPointF scenePos = mNewConnector->startPoint();

		mLinkedBeginElement = getLinkedElement(scenePos);

		if (mLinkedBeginElement != nullptr)
		{
			mLinkedBeginElement->setLinkedHighlight(true, scenePos);
		}
	}
}

void OptimaView::keyPressEvent(QKeyEvent *keyEvent)
{
	if (mNewConnector != nullptr)
	{
		switch(keyEvent->key())
		{
		case Qt::Key_Escape:
			//QCursor::setPos(QPoint(1300,1300));

			delete mNewConnector;
			mNewConnector = nullptr;

			if (mOldConnector != nullptr)
			{
				updateHighlightLinkedElement(QPoint());
				updateHighlightStartLinkedElement();
				mMode = MoveItem;
				mOldConnector->setRebuild(false, false);
				mOldConnector = nullptr;
			}
			
			break;
		}
	}
}

void OptimaView::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
	if (mNewConnector != nullptr)
	{
		onEndDragConnector();
	}
	QGraphicsView::mouseReleaseEvent(mouseEvent);
}

void OptimaView::onEndDragConnector()
{
	if (mOldConnector == nullptr)
	{
		onCreateConnector();
	}
	else
	{
		onRebuildConnector();
	}		

	delete mNewConnector;
	mNewConnector = nullptr;

	updateHighlightLinkedElement(QPoint());
	updateHighlightStartLinkedElement();	

	if (mOldConnector != nullptr)
	{
		mMode = MoveItem;
		mOldConnector = nullptr;
	}
}

void OptimaView::onRebuildConnector()
{
	if (mNewConnector->first() == mOldConnector->first() && mNewConnector->last() == mOldConnector->last())
	{
		return;
	}

	if (mNewConnector->isReversed())
	{
		mOldConnector->setLinked( mLinkedEndElement, mLinkedBeginElement);
	}
	else
	{
		mOldConnector->setLinked( mLinkedBeginElement, mLinkedEndElement);
	}

	mOldConnector->setRebuild(false, false);
	
	mOldConnector->setPoints(mNewConnector->realPoints(mLinkedBeginElement, mLinkedEndElement));
	
	buildIntersectionConnectors();
}

void OptimaView::onCreateConnector()
{
	if (mNewConnector->first() == mNewConnector->last())
	{
		return;
	}

	QDomNode connectorNode = createEmptyConnectorXmlNode(mNewConnector);

	loadElement<OptimaConnector>(connectorNode, true);

	buildIntersectionConnectors();	
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
void OptimaView::loadElement(const QDomNode &element, bool loadAllways)
{
	const QString uuid = element.namedItem( tag::id ).toElement().text();

	if (uuid.isEmpty())
	{			
		if ( !loadAllways )
			throw QString().append( tr( "У элемента не найден UUID" ) );
		return;
	}

	//Найдем на схеме элемент с itemUuid из xml, если его нет, то создадим его
	T *item = getItem<T>(uuid);

	Q_ASSERT( nullptr != item );

	//Запомним переданный или изменим текущий xml элемента и применим результирующий xml 
	//к графическому элементу, после этого он отрисуется на схеме
	item->applyXml(element);

	item->draw(false);
}

template <class T>
void OptimaView::loadElements(const QDomNodeList &elements, bool loadAllways)
{
	for ( int nn = 0; nn < elements.size( ); ++nn )
	{
		loadElement<T>(elements.at( nn ), loadAllways);	
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

QDomNode OptimaView::createEmptyConnectorXmlNode(OptimaTemporaryConnector *tempConnector)
{
	QString text = 
	"\
	<Line>\
		<ID></ID>\
		<PaintLine></PaintLine>\
		<ThicknessLine>1</ThicknessLine>\
		<TypeLine>line</TypeLine>\
		<Order></Order>\
		<IDBegin></IDBegin>\
		<BeginConnectCenter>0</BeginConnectCenter>\
		<IDEnd></IDEnd>\
		<EndConnectCenter>0</EndConnectCenter>\
		<Type>rect</Type>\
		<CrossType>CrossBridge</CrossType>\
		<Linkable>1</Linkable>\
		<RadiusCorner>0</RadiusCorner>\
		<ShapeBegin>NoArrow</ShapeBegin>\
		<ShapeEnd>FilledArrow</ShapeEnd>\
		<SizeShapeBegin>10</SizeShapeBegin>\
		<SizeShapeEnd>10</SizeShapeEnd>\
		<DropShadow>0</DropShadow>\
		<StructureDot>\
		</StructureDot>\
		<StructureGroup></StructureGroup>\
		<UserProperties></UserProperties>\
	</Line>\
	";
	doc.setContent(text);
	
	QString uuid((QUuid::createUuid( ).toString( ).toLower( ).remove( "{" ).remove( "}" )));
	OptimaElement::setXmlValue(doc.documentElement(), tag::id, uuid);

	setXmlValue(doc.documentElement(), tag::id_begin, mLinkedBeginElement == nullptr ? "" : mLinkedBeginElement->uuid());
	setXmlValue(doc.documentElement(), tag::id_end, mLinkedEndElement == nullptr ? "" : mLinkedEndElement->uuid());

	mNewConnector->setPoints(mNewConnector->realPoints(mLinkedBeginElement, mLinkedEndElement));
	OptimaElement::setXmlValue(doc.documentElement(), mNewConnector->points(), uuid);
	OptimaElement::setXmlZOrder(doc.documentElement(), mNewConnector->zValue());

	return doc.documentElement();
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

qreal OptimaView::findNextZOrder()
{
	qreal ret = 0.001;
	QList<QGraphicsItem*> items(scene()->items());

	if (!items.isEmpty())
	{
		ret += items.first()->zValue();
	}

	return ret; 
}
