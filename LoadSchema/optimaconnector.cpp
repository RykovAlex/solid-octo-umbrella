#include "stdafx.h"
#include "optimaconnector.h"
#include "tag.h"
#include "optimaconnectormovemarker.h"

OptimaConnector::OptimaConnector(const QString &itemUuid, OptimaView *view) 
	:OptimaElement(this, itemUuid, view)
	,mBeginArrow(connector_arrow_no, true)
	,mEndArrow(connector_arrow_no, false)
{
	setData(tag::data::linkable, true);
	//для QGraphicView любой путь, даже не замкнутый представляет собой фигуру, он его замыкает сам.
	//у нас коннетор это набор отрезков, поэтому определение, находится ли курсор над коннектором 
	//, будет делать в нашей OptimaView
	
	//setAcceptedMouseButtons(Qt::NoButton);
	setAcceptHoverEvents(true);

	setFlag(ItemIsSelectable);
	setFlag(ItemIsMovable);
}

OptimaConnector::OptimaConnector(OptimaView *view, const OptimaPointVector & points) 
	:OptimaElement(this, "", view)
	,mBeginArrow(connector_arrow_no, true)
	,mEndArrow(connector_arrow_filled, false)
{
	setData(tag::data::linkable, false);
	
	setAcceptedMouseButtons(Qt::NoButton);
	setAcceptHoverEvents(false);
	//setFlag(ItemIsSelectable);
	//setFlag(ItemIsMovable);

	setPen(QPen(Qt::black, 1, Qt::DashDotLine));
	
	setPoints(points);
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

void OptimaConnector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
	//заблокируем стандартное рисование
	//QGraphicsPathItem::paint(painter, option, widget);
	painter->setPen(pen());
	painter->drawPath(path());
	
	painter->setBrush(this->pen().color());
	painter->drawPath(mPathArrow);
}

QVariant OptimaConnector::itemChange(GraphicsItemChange change, const QVariant &value)
{
	switch (change)
	{
	case ItemPositionChange:
		break;
	case ItemMatrixChange:
		break;
	case ItemVisibleChange:
		break;
	case ItemEnabledChange:
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
	case ItemParentChange:
		break;
	case ItemChildAddedChange:
		break;
	case ItemChildRemovedChange:
		break;
	case ItemTransformChange:
		break;
	case ItemPositionHasChanged:
		break;
	case ItemTransformHasChanged:
		break;
	case ItemSceneChange:
		break;
	case ItemVisibleHasChanged:
		break;
	case ItemEnabledHasChanged:
		break;
	case ItemSelectedHasChanged:
		break;
	case ItemParentHasChanged:
		break;
	case ItemSceneHasChanged:
		break;
	case ItemCursorChange:
		break;
	case ItemCursorHasChanged:
		break;
	case ItemToolTipChange:
		break;
	case ItemToolTipHasChanged:
		break;
	case ItemFlagsChange:
		break;
	case ItemFlagsHaveChanged:
		break;
	case ItemZValueChange:
		break;
	case ItemZValueHasChanged:
		break;
	case ItemOpacityChange:
		break;
	case ItemOpacityHasChanged:
		break;
	case ItemScenePositionHasChanged:
		break;
	case ItemRotationChange:
		break;
	case ItemRotationHasChanged:
		break;
	case ItemScaleChange:
		break;
	case ItemScaleHasChanged:
		break;
	case ItemTransformOriginPointChange:
		break;
	case ItemTransformOriginPointHasChanged:
		break;
	default:
		break;
	}
	//throw std::logic_error("The method or operation is not implemented.");
	return value;
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
	setPath( mConnectorPath.toPath() );
	
	//if ( is_selected( ) )
	//{
	//	redraw_coners( redraw_borders );
	//}
}

void OptimaConnector::buildPath()
{
	Q_ASSERT(points().size() >= 2);
	const OptimaCross & cross = mCross;

	//Здесь будут изображения стрелок начала и конца коннетора
	QPainterPath pathArrow;

	// рисуем стрелку начала, и соотвественно меняем начальную точку отрисовки коннектора
	QPointF startPoint = mBeginArrow.getPath(pathArrow, points().at( 0 ), points().at( 1 ));

	mConnectorPath = OptimaPath(startPoint, cross);

	for ( int i = 1; i < points().size( ) - 1; ++i )
	{
		mConnectorPath.lineTo(points().at( i ), QLineF(points().at( i + 1), points().at( i )), mRadiusCorner);
	}

	// рисуем конец
	const QPointF endPoint = mEndArrow.getPath( pathArrow, points().at( points().size() - 1 ), points().at( points().size() - 2 ));

	mConnectorPath.lineTo(endPoint);

	// сохраним для отрисовки в paint()
	mPathArrow = pathArrow;
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

void OptimaConnector::setPoints(const OptimaPointVector & val)
{
	mPoints = val; 	
	
	buildPath(); 
	
	draw();
}

QPainterPath OptimaConnector::shape() const
{
	QPainterPath path(this->path());
	QPainterPathStroker pathStrocke;
	pathStrocke.setWidth(20);

	return pathStrocke.createStroke(path);
}

void OptimaConnector::onLineMove(const OptimaConnectorMoveMarker* moveMarker)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void OptimaConnector::onMarkerMove(const OptimaBaseMarker* marker)
{
	const OptimaConnectorMoveMarker* moveMarker = dynamic_cast<const OptimaConnectorMoveMarker*>(marker);
	
	if (moveMarker != nullptr)
	{
		onLineMove(moveMarker);
	}
}

void OptimaConnector::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	//QGraphicsItem::mousePressEvent(event);
}

void OptimaConnector::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	Q_ASSERT(mView != nullptr);
	
	//QRectF r(event->pos() - QPointF(5.0,5.0),event->pos() + QPointF(5.0,5.0));
	//if (!shape().intersects(r))
	//	return;

	QPen newPen(pen());
	newPen.setWidth(newPen.width() + 1);
	setPen(newPen) ;
}

void OptimaConnector::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	Q_ASSERT(mView != nullptr);

	QPen newPen(pen());
	newPen.setWidth(newPen.width() - 1);
	//newPen.setWidth(1);
	setPen(newPen) ;
}

void OptimaConnector::createMarkers()
{
	// создадим маркеры посередине и на углах неуглового коннетора
	// эти марверы по умолчанию представляют из субя булые квалраты с 
	// зеленой окантовкой
	for ( int i = 0; i < points().size() - 1; ++i )
	{
		Qt::CursorShape сursorShape(Qt::SizeAllCursor);
		
		if ( mIsAngledСonnector )
		{
			// выясним поведение будушего маркера для углового коннектора
			const int r1 = mView->getEntireCellsQnt( points().at( i ).x( ));
			const int r2 = mView->getEntireCellsQnt( points().at( i + 1 ).x( ));
			сursorShape = r1 == r2 ? Qt::SizeHorCursor : Qt::SizeVerCursor;
		}
		else
		{
			// у прямого коннетора поведение маркеров одинаково что посередине, что на точках
			if ( i > 0 )
			{
				new OptimaConnectorMoveMarker( this, points().at(i), сursorShape);
			}
		}
		
		// это середина отрезка
		new OptimaConnectorMoveMarker( this, (points().at(i) + points().at(i+1)) / 2, сursorShape);
	}
}

void OptimaConnector::destroyMarkers()
{
	QList<QGraphicsItem *> items = childItems();
	while(!items.isEmpty())
	{
		delete items.takeFirst();
	}
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
