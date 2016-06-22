#include "stdafx.h"
#include "tag.h"
#include "optimaconnector.h"
#include "optimaview.h"
#include "optimafigure.h"
#include "optimatext.h"

OptimaView::OptimaView(QWidget *parent) 
	: QGraphicsView(parent)
	, mMode(MoveItem)
	, newConnector(nullptr)
{ 
	setScene( new QGraphicsScene(parent) );
	//scene()->setSceneRect(QRectF(0.0, 0.0, 3000.0,3000.0));
	//setMouseTracking(true);
};

void OptimaView::setMode(Mode mode)
{
	mMode = mode;
}

void OptimaView::createNewConnector(QMouseEvent * mouseEvent)
{
	if (newConnector)
	{
		delete newConnector;
	}
	newConnector = new QGraphicsLineItem(QLineF( mapToScene(mouseEvent->pos()), mapToScene(mouseEvent->pos())));
	//QGraphicsLineItem *newConnector = new QGraphicsLineItem(QLineF( mouseEvent->pos(), mouseEvent->pos()));

	newConnector->setPen(QPen(Qt::black, 1, Qt::DashDotLine));
	scene()->addItem(newConnector);
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
		createNewConnector(mouseEvent);
		break;
	case InsertText:
		break;
	case MoveItem:
		break;
	default:
		break;
	}
	QGraphicsView::mousePressEvent(mouseEvent);
}

void OptimaView::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
	if (newConnector != nullptr)
	{
		delete newConnector;
		newConnector = nullptr;
	}
	QGraphicsView::mouseReleaseEvent(mouseEvent);
}

void OptimaView::mouseMoveEvent(QMouseEvent *mouseEvent)
{
	if (newConnector != nullptr )
	{
		QLineF newLine(newConnector->line().p1(), mapToScene(mouseEvent->pos()));
		newConnector->setLine(newLine);
	} 
	else
	{
		QGraphicsView::mouseMoveEvent(mouseEvent);
	}
	
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

bool OptimaView::isConnector(const QGraphicsItem* item) const
{
	return (dynamic_cast<const OptimaConnector*>(item) != nullptr);
}
