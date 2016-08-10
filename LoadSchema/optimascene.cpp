#include "stdafx.h"
#include "tag.h"
#include "optimascene.h"
#include "OptimaConnectorMarker.h"
#include "optimaconnector.h"
#include "optimatext.h"
#include "optimaconnectorbordermarkerend.h"

OptimaScene::OptimaScene(QUndoStack *stack) 
	:QGraphicsScene()
	, mNewConnectorEndMarker(nullptr)
	, mMode(MoveItem)
	, mHoverElement(nullptr)
	, mStack(stack)
{

}

OptimaScene::~OptimaScene()
{

}


void OptimaScene::updateHighlightElement(QPointF scenePos)
{
	if (mHoverElement != nullptr )
	{
		mHoverElement->setLinkedHighlight(false);
	}

	mHoverElement = getLinkedElement(scenePos);

	if (mHoverElement != nullptr)
	{
		mHoverElement->setLinkedHighlight(true, scenePos);
	}
}

QDomNode OptimaScene::createConnectorXmlNode(OptimaConnectorMarker *marker)
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
	mDoc.setContent(text);

	QString uuid((QUuid::createUuid( ).toString( ).toLower( ).remove( "{" ).remove( "}" )));
	OptimaXml::setXmlValue(mDoc.documentElement(), tag::id, uuid);

	setXmlValue(mDoc.documentElement(), tag::id_begin, marker->getLinkedBeginUuid());
	setXmlValue(mDoc.documentElement(), tag::id_end, marker->getLinkedEndUuid());

	OptimaXml::setXmlValue(mDoc.documentElement(), marker->getNewConnectorPoints(), uuid);
	OptimaElement::setXmlZOrder(mDoc.documentElement(), marker->getNewConnectorZValue());

	return mDoc.documentElement();
}

void OptimaScene::addNewConnector(OptimaConnectorMarker *marker)
{
	if (!marker->isNewConnectorPresent())
		return;

	QDomNode connectorNode = createConnectorXmlNode(marker);

	loadElement<OptimaConnector>(connectorNode, true);

	buildIntersectionConnectors();
}

void OptimaScene::before1CCall()
{

}

void OptimaScene::loadWorkspace(const QDomNodeList &workspace)
{
	for ( int nn = 0; nn < workspace.size( ); ++nn )
	{
		const QDomNode &element = workspace.at( nn );

		//Запомним переданный или изменим текущий xml элемента и применим результирующий xml 
		//к графическому элементу, после этого он отрисуется на схеме
		applyXml(element, QDomNode());

		//draw(true);
	}
}

void OptimaScene::loadNoWorkspace(const QDomNodeList & noWorkspace)
{
	for ( int nn = 0; nn < noWorkspace.size( ); ++nn )
	{
		const QDomNode &element = noWorkspace.at( nn );

		//Запомним переданный или изменим текущий xml элемента и применим результирующий xml 
		//к графическому элементу, после этого он отрисуется на схеме
		mNoWorkspace.applyXml(element, QDomNode());
	}
}

int OptimaScene::getEntireCellsQnt(qreal coordinate) const
{
	return qRound( coordinate / mAlignGridStep );
}

QPoint OptimaScene::alignToGrid(const QPoint& pos) const
{
	return QPoint( alignToGrid( pos.x() ), alignToGrid( pos.y() ) );
}

QPointF OptimaScene::alignToGrid(const QPointF& pos) const
{
	return QPointF( alignToGrid( pos.x() ), alignToGrid( pos.y() ) );
}

qreal OptimaScene::alignToGrid(const qreal & coordinate) const
{
	return mAlignGridStep * getEntireCellsQnt( coordinate );
}

void OptimaScene::setMode(Mode mode)
{
	mMode = mode;
}

OptimaElement * OptimaScene::getLinkedElement(const QString & elementUuid)
{
	return dynamic_cast<OptimaElement*>(findItem(elementUuid));
}

OptimaElement * OptimaScene::getLinkedElement(const QPointF & scenePos) const
{
	return dynamic_cast<OptimaElement*>(OptimaConnectorPathFinder::findLinkedItem(this, scenePos));
}

QString OptimaScene::getUuid(QGraphicsItem* item)
{
	return item->data(tag::data::uuid).toString();
}

QGraphicsItem * OptimaScene::findItem(const QString &findItemUuid)
{
	if (findItemUuid.isEmpty())
	{
		return nullptr;
	}

	const QList<QGraphicsItem*> itemList = items();
	for (QList<QGraphicsItem*>::const_iterator i = itemList.constBegin(); i != itemList.constEnd(); ++i )
	{
		QGraphicsItem* item = *i;
		QString itemUuid = getUuid(item);
		if (itemUuid == findItemUuid)
		{
			return item;
		}
	}

	return nullptr;
}

void OptimaScene::buildIntersectionConnectors()
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

QString OptimaScene::LoadScheme(const QString &xml, const QString &xmlPattern, bool load_allways)
{
	before1CCall();

	try
	{
		int xml_error_column;
		int xml_error_line;
		QString xml_error_msg;

		if (!xmlPattern.isEmpty())
		{
			if ( !mPatternDoc.setContent( xmlPattern, &xml_error_msg, &xml_error_line, &xml_error_column ) )
				throw QString().append( tr( "Ошибка при анализе xml шаблона, сообщение: %1, строка: %2, столбец: %3\n" ).arg( xml_error_msg ).arg( xml_error_line ).arg( xml_error_column ) );
		}

		if ( !mDoc.setContent( xml, &xml_error_msg, &xml_error_line, &xml_error_column ) )
			throw QString().append( tr( "Ошибка при анализе xml, сообщение: %1, строка: %2, столбец: %3\n" ).arg( xml_error_msg ).arg( xml_error_line ).arg( xml_error_column ) );

		const QDomElement docElement = mDoc.documentElement( );
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
		//Загружаем нерабочее пространство
		loadNoWorkspace(docElement.elementsByTagName( tag::no_workspace ));
		//Загружаем рабочее пространство
		loadWorkspace(docElement.elementsByTagName( tag::noWorkspace ));

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

qreal OptimaScene::findNextZOrder() const
{
	qreal ret = 0.001;
	QList<QGraphicsItem*> items(items());

	if (!items.isEmpty())
	{
		ret += items.first()->zValue();
	}

	return ret;
}

QString OptimaScene::SaveScheme()
{
	QString xmlSchema("<Schema>\r\n");
	const QList<QGraphicsItem*> itemList = items();
	
	xmlSchema += mNoWorkspace.getXmlString();
	xmlSchema += getXmlString();
	
	//очистку приходится делать отдельно, так как пересечения с углами не подчиняется правилу zOrder
	for (QList<QGraphicsItem*>::const_iterator i = itemList.constBegin(); i != itemList.constEnd(); ++i )
	{
		OptimaElement* element = dynamic_cast<OptimaElement*>(*i);
		if (element == nullptr)
		{
			continue;
		}

		xmlSchema += element->getXmlString();
	}
	xmlSchema += QString("</Schema>");
	qDebug() << xmlSchema;
	
	return xmlSchema;
}

void OptimaScene::pushUndoCommand(QUndoCommand *undoCommand)
{
	mStack->push(undoCommand);
}

void OptimaScene::keyPressEvent(QKeyEvent *event)
{
	switch(event->key())
	{
	case Qt::Key_Escape:
		if (mNewConnectorEndMarker == nullptr)
		{
			return;
		}
		delete mNewConnectorEndMarker;
		mNewConnectorEndMarker = nullptr;

		break;
	}
}

void OptimaScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF scenePos = event->scenePos();

	if (event->button() != Qt::LeftButton)
		return;

	switch(mMode)
	{
	case InsertItem:
		break;
	case InsertLine:
		//addConnector(mouseEvent);
		mNewConnectorEndMarker =  new OptimaConnectorBorderMarkerEnd(this, scenePos);
		return;
	case InsertText:
		break;
	case MoveItem:
		break;
	default:
		break;
	}
	QGraphicsScene::mousePressEvent(event);
}

void OptimaScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF eventPos = event->pos();

	QPointF scenePos = event->scenePos();

	//подсветка областей к которым будем коннектиться
	if (mMode == InsertLine)
	{
		if (mNewConnectorEndMarker != nullptr)
		{
			mNewConnectorEndMarker->setMarkerPos(scenePos);
		}
		else
		{
			updateHighlightElement(scenePos);
		}
		QGraphicsScene::mouseMoveEvent(event);
	}
	else
	{
		QGraphicsScene::mouseMoveEvent(event);
	}
}

void OptimaScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (mNewConnectorEndMarker != nullptr)
	{
		addNewConnector(mNewConnectorEndMarker);

		delete mNewConnectorEndMarker;
		mNewConnectorEndMarker = nullptr;
	}

	QGraphicsScene::mouseReleaseEvent(event);
}

QDomNode OptimaScene::getPatternNode(QString namePattern)
{
	if (namePattern.isEmpty())
	{
		return QDomNode();
	}
	
	QDomNodeList collections = mPatternDoc.documentElement().elementsByTagName(tag::collection);
	for ( int i = 0; i < collections.size( ); ++i )
	{
		QDomNodeList patterns = collections.at(i).toElement().elementsByTagName(tag::name_pattern);

		for ( int i = 0; i < patterns.size( ); ++i )
		{
			if (patterns.at(i).attributes().namedItem(tag::name).nodeValue() == namePattern)
			{
				return patterns.at(i).firstChildElement();
			}		
		}
	}

	return QDomNode();
}

void OptimaScene::apply()
{
	// переменные которые не будут изменяться пользователем
	mAlignGridStep = getXmlValue(tag::virtual_step, 4.0);
}

template <class T>
void OptimaScene::loadElement(const QDomNode &element, bool loadAllways)
{
	const QString uuid = element.namedItem( tag::id ).toElement().text();
	const QString name_pattern = element.namedItem( tag::name_pattern ).toElement().text();

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
	item->applyXml(element, getPatternNode(name_pattern));

	item->draw(false);
}

template <class T>
void OptimaScene::loadElements(const QDomNodeList &elements, bool loadAllways)
{
	for ( int nn = 0; nn < elements.size( ); ++nn )
	{
		loadElement<T>(elements.at( nn ), loadAllways);	
	}
}

template <class T>
T * OptimaScene::getItem(const QString &itemUuid)
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

	T *item = new T(itemUuid);
	addItem(item);			

	return item;
}
