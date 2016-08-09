#include "stdafx.h"
#include "tag.h"
#include "optimascene.h"
#include "optimaconnector.h"
#include "OptimaConnectorMarker.h"
#include "optimaconnectorbordermarkerend.h"

OptimaConnector::OptimaConnector(const QString &itemUuid) 
	:OptimaElement(this, itemUuid)
	,OptimaTemporaryConnector()
	,mRebuild(false)
{
	initialize();
}

void OptimaConnector::setBorderId(const QString &nameId, const OptimaElement *element)
{
	setXmlValue(nameId, element == nullptr ? "" : element->uuid());
}

bool OptimaConnector::checkBorderLinking(const QString &nameId)
{
	return !getXmlValue(nameId, QString("")).isEmpty();
}

OptimaElement *OptimaConnector::getBorderLinking(const QString &nameId)
{
	OptimaScene *optimaScene = dynamic_cast<OptimaScene*>(scene());
	Q_ASSERT (NULL != optimaScene);

	return optimaScene->getLinkedElement(getXmlValue(nameId, QString("")));
}

OptimaScene * OptimaConnector::scene()
{
	OptimaScene *optimaScene = dynamic_cast<OptimaScene *>(QGraphicsItem::scene());
	Q_ASSERT(NULL != optimaScene);

	return optimaScene;
}

void OptimaConnector::hideMarkers(const OptimaConnectorLineMarker* lineMarker, bool hide)
{
	QList<QGraphicsItem *> items = childItems();
	for (int i = 0; i < items.count(); ++i)
	{
		QGraphicsItem *item = items.at(i);
		if (item != lineMarker)
		{
			item->setVisible(false);
		}
	}
}

void OptimaConnector::initialize()
{
	setData(tag::data::linkable, true);

	setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
	setAcceptHoverEvents(true);

	setFlag(ItemIsSelectable);
	setFlag(ItemIsMovable);
	setFlag(ItemSendsGeometryChanges);

	setPen(QPen(Qt::black, 1, Qt::SolidLine));

	mIsAngledСonnector = true;
	setCursor(QCursor( QPixmap( ":/images/resources/cursor_move_rect.png"), 0, 0));
	mNonSelectedZOrder = zValue();
}

OptimaConnector::~OptimaConnector()
{

}

void OptimaConnector::apply()
{
	//1. Заполним рабочие переменные
	//Эти переменные выделены из XML потому что испльзуются при отрисовке
	//Значение неизменно. не требует изменения XML при сохранении элемента
	getXmlValue(tag::shape_begin, mBeginArrow);
	mBeginArrow.setSize(getXmlValue(tag::size_shape_begin, 10.0));

	getXmlValue(tag::shape_end, mEndArrow);
	mEndArrow.setSize(getXmlValue(tag::size_shape_end, 10.0));

	getXmlValue(tag::cross_type, mCross);
	
	mRadiusCorner = getXmlValue(tag::radius_corner, 0.0);
	
	//получаем тип коннетора
	mIsAngledСonnector = getXmlValue(tag::type, QString("direct")) == "rect";
	setCursor(QCursor( QPixmap( mIsAngledСonnector ? ":/images/resources/cursor_move_rect.png" : ":/images/resources/cursor_move_direct.png"), 0, 0));

	//Значения этих переменных вынесены в переменные, потому что они интерактивно изменяются пользователем,
	//Требуется изменить XML перед сохранением, длясохранения действий пользователя
	//Непосредственное испрользование из XML требует затрат процессора на постоянное извлечение и перезапись,
	//нерационально
	getXmlValue(tag::structure_dot, mPoints );

	//Зная точки построим непосредственно коннектор, передав в качестве параметра класс отвечающий за рисовангия пересечений
	buildPath();
}

void OptimaConnector::draw(bool isProcessLoading /*= false*/)
{
	//нет смысла рисовать коннетор до загрузки всех коннеткоров и определения точек их пересечения
	//для коннекторов будет вызвана draw еще раз, после определения точек пересечения с другими коннеторами
	if (isProcessLoading)
	{
		return;
	}

	// обозначим пространство, в котором будем рисовать коннетор
	// setPath( mConnectorPath.toPath() );
	OptimaTemporaryConnector::draw();
}

QVariant OptimaConnector::itemChange(GraphicsItemChange change, const QVariant &value)
{
	switch (change)
	{
	case ItemPositionHasChanged:
		break;
	case ItemPositionChange:

		break;
	case ItemSelectedChange:
		if (value == true)
		{
			createMarkers();
		} 
		else
		{
			destroyMarkers();
		}
		break;
	default:
		break;
	}

	return value;
}

void OptimaConnector::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	OptimaScene *optimaScene = dynamic_cast<OptimaScene*>(scene());
	Q_ASSERT (NULL != optimaScene);

	if (!pos().isNull())
	{
		setFlag(ItemSendsGeometryChanges, false);
		onConnectorMove(pos());

		buildPath();
		
		optimaScene->buildIntersectionConnectors();

		setFlag(ItemSendsGeometryChanges, true);
	}
	
	QGraphicsItem::mouseReleaseEvent(event);
}

void OptimaConnector::intersected(OptimaPath & connectorPath)
{
	mConnectorPath.intersected(connectorPath);
}

void OptimaConnector::getIntersection(const QList<QGraphicsItem*> &itemList, int start)
{
	for (QList<QGraphicsItem*>::const_iterator i = itemList.constBegin() + start; i != itemList.constEnd(); ++i )
	{
		OptimaConnector* item = dynamic_cast<OptimaConnector*>(*i);
		if (item == nullptr)
		{
			continue;
		}

		// определим какой конннетор выше, он и будет запоминать точки
		// так как при пересечении угла не важно какой элемент выше, то в item->mConnectorPath
		// также могут быть добавленый точки пересечения с углами этого коннетора
		if (zValue() > item->zValue())
		{
			intersected(item->mConnectorPath);
		}
		else
		{
			item->intersected(mConnectorPath);
		}		
	}
}

void OptimaConnector::clearIntersection()
{
	mConnectorPath.clearIntersection();
}

int OptimaConnector::type() const
{
	return Type;
}

void OptimaConnector::setLinkedHighlight(bool enabled, const QPointF & scenePos /*= QPointF()*/)
{
	if (enabled)
	{
		checkLinkedHighlight(scenePos, mIndexLinkedLine);
	}
	else
	{
		mIndexLinkedLine = -1;
	}


	setData(tag::data::linkingParameter, mIndexLinkedLine);
	
	update();
}

bool OptimaConnector::checkLinkedHighlight(const QPointF & scenePos, int &indexLinkedLine) const
{
	indexLinkedLine = -1;
	for (int i = 0; i < (mPoints.size() - 1); ++i)
	{
		QLineF line(getPathLine(i));
		line.translate(pos());

		QPainterPath strokeLinePath(getStrokeLinePath(line));
		if (strokeLinePath.contains(scenePos))
		{
			indexLinkedLine = i;
			return true;
		}
	}

	return false;
}

bool OptimaConnector::getLinkedLine(const QPointF & scenePos, OptimaLine &linkedLine) const
{
	int indexLinkedLine = -1;
	
	if (checkLinkedHighlight(scenePos, indexLinkedLine))
	{
		linkedLine = OptimaLine( mPoints.at(indexLinkedLine), mPoints.at(indexLinkedLine + 1), indexLinkedLine);
		return true;
	}

	return false;
}

bool OptimaConnector::checkLinkedHighlight(const QPointF & scenePos)
{
	int indexLinkedLine;

	return checkLinkedHighlight(scenePos, indexLinkedLine);
}

QPainterPath OptimaConnector::getStrokeLinePath(const QLineF &line) const
{
	QPainterPath linePath(line.p1());
	linePath.lineTo(line.p2());

	QPainterPathStroker pathStrocke;
	pathStrocke.setWidth(margin);

	return pathStrocke.createStroke(linePath);
}

void OptimaConnector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
	if (mRebuild)
	{
		painter->setOpacity(0.25);
	}

	OptimaTemporaryConnector::paint(painter, option);

	//if (mIndexLinkedLine >= 0)
	//{
	//	painter->setOpacity(0.25);
	//	painter->setPen(Qt::red);
	//	//painter->drawRect(boundingRect());
	//	//painter->setPen(Qt::NoPen);

	//	painter->setBrush(QBrush(Qt::gray));
	//	painter->setOpacity(0.25);
	//	painter->drawPath(getStrokeLinePath(getPathLine(mIndexLinkedLine)));
	//}
	if (isSelected())
	{
		painter->setPen(Qt::NoPen);
		painter->setBrush(QBrush(Qt::gray));
		painter->setOpacity(0.25);
		//QPainterPath path(this->path());
		QPainterPath path(mConnectorCleanPath.toPath());
		QPainterPathStroker pathStrocke;
		pathStrocke.setCapStyle(Qt::RoundCap);
		pathStrocke.setJoinStyle(Qt::RoundJoin);
		pathStrocke.setWidth(margin);
		painter->drawPath(pathStrocke.createStroke(path));
		
	}
}		

void OptimaConnector::setRebuild(bool val)
{
	mRebuild = val;
	setData(tag::data::linkable, !mRebuild);
	update();
}

void OptimaConnector::onLineMove(const OptimaConnectorLineMarker* lineMarker)
{
	int indexLine = lineMarker->getIndexLine();
	
	Q_ASSERT( indexLine >= 0 && indexLine < mPoints.size() - 1 );

	acceptNewPoint(lineMarker->p2(), indexLine + 1, indexLine + 1);
	acceptNewPoint(lineMarker->p1(), indexLine, indexLine + 1);

	buildPath();

	draw();

	//Q_ASSERT(childItems().count() >= 2);
	//hideMarkers(lineMarker, false);
}

void OptimaConnector::acceptNewPoint(OptimaPoint p, int indexLine, int insertPosition)
{
	if (mPoints.at(indexLine).uuid() == p.uuid())
	{
		mPoints[indexLine] = p;
	}
	else
	{
		mPoints.insert(insertPosition, p);
	}
}

void OptimaConnector::onMarkerMove(const OptimaBaseMarker* marker)
{
	const OptimaConnectorMoveMarker* moveMarker = dynamic_cast<const OptimaConnectorMoveMarker*>(marker);
	const OptimaConnectorLineMarker* lineMarker = dynamic_cast<const OptimaConnectorLineMarker*>(marker);	
	if (moveMarker != nullptr)
	{

	}
	if (lineMarker != nullptr)
	{
		onLineMove(lineMarker);
	}
}

void OptimaConnector::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	//QGraphicsItem::mousePressEvent(event);
}

void OptimaConnector::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	if (!isSelected())
	{
		createMarkers();
	}

	QPen newPen(pen());
	newPen.setWidth(newPen.width() + 1);
	setPen(newPen) ;
}

void OptimaConnector::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	if (!isSelected())
	{
		destroyMarkers();
	}
	
	
	QPen newPen(pen());
	newPen.setWidth(newPen.width() - 1);
	setPen(newPen) ;
}

void OptimaConnector::createMarkers()
{
	OptimaScene *optimaScene = dynamic_cast<OptimaScene*>(scene());
	Q_ASSERT (NULL != optimaScene);

	// возможно маркеры уже присутствуют
	if (!childItems().isEmpty())
	{
		return;
	}

	// создадим маркеры начала коннетора и его окончания
	OptimaConnectorBorderMarkerEnd *markerEnd = new OptimaConnectorBorderMarkerEnd( this );
	//new OptimaConnectorMarker( this, mPoints.last(), Qt::SizeAllCursor, false, mView->getLinkedElement(mPoints.last()) );

	// создадим маркеры посередине и на углах неуглового коннетора
	// эти марверы по умолчанию представляют из себя белые квалраты с 
	// зеленой окантовкой
	for ( int i = 0; i < mPoints.size() - 1; ++i )
	{
		Qt::CursorShape сursorShape(Qt::SizeAllCursor);
		
		if ( mIsAngledСonnector )
		{
			// выясним поведение будушего маркера для углового коннектора
			const int r1 = optimaScene->getEntireCellsQnt( mPoints.at( i ).x( ));
			const int r2 = optimaScene->getEntireCellsQnt( mPoints.at( i + 1 ).x( ));
			сursorShape = r1 == r2 ? Qt::SizeHorCursor : Qt::SizeVerCursor;

			new OptimaConnectorLineMarker( this, OptimaLine(mPoints.at(i),mPoints.at(i+1), i), сursorShape);
		}
		else
		{
			// у прямого коннетора поведение маркеров одинаково что посередине, что на точках
			if ( i > 0 )
			{
				new OptimaConnectorMoveMarker( this, mPoints.at(i), сursorShape);
				// это середина отрезка прямого коннеткора
				new OptimaConnectorMoveMarker( this, (mPoints.at(i) + mPoints.at(i+1)) / 2, сursorShape);
			}
		}
		
	}
	
	mNonSelectedZOrder = zValue();
	setZValue(100 + mNonSelectedZOrder);
}

bool OptimaConnector::destroyMarkers()
{
	//проверим что есть что удалять
	QList<QGraphicsItem *> items = childItems();
	if (items.isEmpty())
	{
		return false;
	}
	//первым маркером является маркер конца, при уничтожении 
	//он удаляем маркер начала, поэтому нужно освежить список
	delete items.takeFirst();
	items = childItems();
	while(!items.isEmpty())
	{
		delete items.takeFirst();
	}

	setZValue(mNonSelectedZOrder);

	return true;
}

void OptimaConnector::onConnectorMove(QPointF deltaPoint)
{
	for (int i=0; i < mPoints.size(); ++i)
	{
		mPoints[i] += deltaPoint;
	}
	rebuildMarkers();
}

void OptimaConnector::setPoints(const OptimaPointVector & val)
{
	OptimaTemporaryConnector::setPoints(val);
	rebuildMarkers();
}

void OptimaConnector::rebuildMarkers()
{
	if (destroyMarkers())
	{
		createMarkers();
	}
}

QPointF OptimaConnector::getIntersectPoint(const QLineF line) const
{
	for(int i = 0; i < mPoints.size() - 1; ++i)
	{
		QPointF intersectPoint;
		QLineF figureLine(mPoints.at(i), mPoints.at(i+1));
		if (figureLine.intersect(line, &intersectPoint) == QLineF::BoundedIntersection)
		{
			return intersectPoint;
		}		
	}

	return line.p1();
}

void OptimaConnector::setLinked(OptimaElement * linkedBeginElement, OptimaElement * linkedEndElement)
{
	setXmlValue( tag::id_begin, linkedBeginElement == nullptr ? "" : linkedBeginElement->uuid());
	setXmlValue( tag::id_end, linkedEndElement == nullptr ? "" : linkedEndElement->uuid());
}

//connector_move_marker * connector_controller::create_move_marker( int index_point )
//{
//	const int r1 = m_workspace.round_val( m_points.at( index_point ).m_pos.x( ) );
//	const int r2 = m_workspace.round_val( m_points.at( index_point + 1 ).m_pos.x( ) );
//	bool eq_by_x = ( r1 == r2 );
//	const int directrion = ( m_type == connector_direct ? 4 : ( eq_by_x ? 0 : 1 ) );
//
//	return create_connector_move_marker( directrion, ( m_points.at( index_point ).m_pos + m_points.at( index_point + 1 ).m_pos ) / 2, Qt::green );
//}

//void OptimaConnector::create_connector_move_marker( int direction, const QPointF &pos, const QColor border_color )
//{
//	connector_move_marker * ret = new connector_move_marker( direction, this );
//	ret->setPos( pos );
//	ret->setRect( get_marker_rect() );
//	ret->setZValue( 0.999998 );
//	QPen gp( border_color, 1.0 );
//	gp.setCosmetic( true );
//	ret->setPen( gp );			
//
//	ret->setBrush( Qt::white );
//
//	m_scene->addItem( ret );
//	ret->block_movement = false;
//
//	return ret;
//}
//
//void OptimaConnector::setSelected( bool sel )
//{
//	if ( isSelected() )
//	{
//		m_is_selected = sel;
//		if ( sel )
//		{
//			redraw_coners( true );
//			// рисуем квадраты что бы тянуть
//			for ( int nn = 0; nn < m_points.size( ) - 1; ++nn )
//			{
//				if ( m_type == connector_direct	&& nn >= 1 )
//				{
//					m_movers.push_back( create_connector_move_marker( 4, m_points.at( nn ).m_pos, Qt::green ) );
//				}
//				// это середина отрезка
//				m_movers.push_back( create_move_marker( nn ) );
//			}
//		}
//		else
//		{
//			QList<QGraphicsItem *> items = childItems();
//			while(!items.isEmpty())
//			{
//				delete items.takeFirst();
//			}
//		}
//	}
//}
