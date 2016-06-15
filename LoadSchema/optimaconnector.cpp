#include "stdafx.h"
#include "optimaconnector.h"
#include "tag.h"
#include "optimaconnectormovemarker.h"

OptimaConnector::OptimaConnector(const QString &itemUuid, OptimaView *view) 
	:OptimaElement(this, itemUuid, view)
	,mBeginArrow(connector_arrow_no, true)
	,mEndArrow(connector_arrow_no, false)
	,mIsHighlight(false)
{
	//для QGraphicView любой путь, даже не замкнутый представляет собой фигуру, он его замыкает сам.
	//у нас коннетор это набор отрезков, поэтому определение, находится ли курсор над коннектором 
	//, будет делать в нашей OptimaView
	
	//setAcceptedMouseButtons(Qt::NoButton);
	setAcceptHoverEvents(true);

	setFlag(ItemIsSelectable);
	setFlag(ItemIsMovable);
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

	//Сохраняем текущий карандаш, так как надо отрабатывать выделение коннеторов при подводе к ним мышки
	//Начальные настройки карандаша устанавливаюся в applyXml
	mPen = pen();	
	
	//Сделаем будто бы коннетор широкий, но на самом деле, таким его рисовать не будем
	QPen localPen(mPen);
	localPen.setWidth(10);
	setPen(localPen);

	//Значения этих переменных вынесены в переменные, потому что они интерактивно изменяются пользователем,
	//Требуется изменить XML перед сохранением, длясохранения действий пользователя
	//Непосредственное испрользование из XML требует затрат процессора на постоянное извлечение и перезапись,
	//нерационально
	getXmlValue(tag::structure_dot, mPoints );

	//Зная точки построим непосредственно коннектор, передав в качестве параметра класс отвечающий за рисовангия пересечений
	buildPath(mCross);
}

void OptimaConnector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
	QPen localPen(mPen);

	if (mIsHighlight)
	{
		localPen.setWidth(localPen.width() + 1);
	}
	
	//заблокируем стандартное рисование
	//QGraphicsPathItem::paint(painter, option, widget);
	
	painter->setPen(localPen);
	painter->drawPath(mPathConnector);

	painter->setBrush(this->pen().color());
	painter->drawPath(mPathArrow);
}

void OptimaConnector::draw(bool isProcessLoading /*= false*/)
{	
	//нет смысла рисовать коннетор до загрузки всех коннеткоров и определения точек их пересечения
	//для коннекторов будет вызвана draw еще раз, после определения точек пересечения с другими коннеторами
	if (isProcessLoading)
	{
		return;
	}

	// сохраним для отрисовки в paint()
	mPathConnector = mConnectorPath.toPath();

	// обозначим пространство, в котором будем рисовать коннетор
	setPath( mSensitiveArea.toPath() );
	
	//if ( is_selected( ) )
	//{
	//	redraw_coners( redraw_borders );
	//}
}

void OptimaConnector::buildPath(const OptimaCross & cross)
{
	Q_ASSERT(mPoints.size() >= 2);

	//Здесь будут изображения стрелок начала и конца коннетора
	QPainterPath pathArrow;

	// рисуем стрелку начала, и соотвественно меняем начальную точку отрисовки коннектора
	QPointF startPoint = mBeginArrow.getPath(pathArrow, mPoints.at( 0 ), mPoints.at( 1 ));

	mConnectorPath = OptimaPath(startPoint, cross);
	mSensitiveArea = OptimaPath(mPoints.first());

	for ( int i = 1; i < mPoints.size( ) - 1; ++i )
	{
		mConnectorPath.lineTo(mPoints.at( i ), QLineF(mPoints.at( i + 1), mPoints.at( i )), mRadiusCorner);
		mSensitiveArea.lineTo(mPoints.at( i ), QLineF(mPoints.at( i + 1), mPoints.at( i )), mRadiusCorner);
	}

	// рисуем конец
	const QPointF endPoint = mEndArrow.getPath( pathArrow, mPoints.at( mPoints.size() - 1 ), mPoints.at( mPoints.size() - 2 ));

	mConnectorPath.lineTo(endPoint);
	mSensitiveArea.lineTo(mPoints.last());

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
	
	mIsHighlight = true;
	update();
}

void OptimaConnector::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	Q_ASSERT(mView != nullptr);

	mIsHighlight = false;
	update();
}

void OptimaConnector::setSelected( bool sel )
{
	if ( is_selected( ) != sel )
	{
		m_is_selected = sel;
		if ( sel )
		{
			redraw_coners( true );
			// рисуем квадраты что бы тянуть
			for ( int nn = 0; nn < m_points.size( ) - 1; ++nn )
			{
				if ( m_type == connector_direct	&& nn >= 1 )
				{
					//connector_move_marker * ret = new connector_move_marker( 4, this );
					//ret->setPos( m_points.at( nn ).m_pos );
					//ret->setRect( get_marker_rect() );
					//ret->setZValue( 0.999998 );
					//QPen gp( Qt::green, 1.0 );
					//gp.setCosmetic( true );
					//ret->setPen( gp );			
					//ret->setBrush( Qt::green );
					//m_scene->addItem( ret );
					//ret->block_movement = false;
					//m_movers.push_back( ret );
					m_movers.push_back( create_connector_move_marker( 4, m_points.at( nn ).m_pos, Qt::green ) );
				}
				// это середина отрезка
				m_movers.push_back( create_move_marker( nn ) );
			}
		}
		else
		{
			clear_items( m_crosses );
			clear_items( m_bor_movers );
			clear_items( m_movers );
		}
	}
}
