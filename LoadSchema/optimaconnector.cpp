#include "stdafx.h"
#include "optimaconnector.h"
#include "tag.h"
#include "OptimaCross.h"

OptimaConnector::OptimaConnector(const QString &itemUuid) 
	:OptimaElement(this, itemUuid)
	,mBeginArrow(connector_arrow_no, true)
	,mEndArrow(connector_arrow_no, false)
{
}

void OptimaConnector::apply(const QDomNode & connector)
{
	//1. Примем XML
	applyXml(connector);

	//2. Заполним рабочие переменные
	//Эта переменная выделена из XML потому что испльзуются при отрисовке
	//Значение неизменно. не требует изменения XML при сохранении элемента
	getXmlValue(tag::shape_begin, mBeginArrow);
	mBeginArrow.setSize(getXmlValue(tag::size_shape_begin, 10.0));

	getXmlValue(tag::shape_end, mEndArrow);
	mEndArrow.setSize(getXmlValue(tag::size_shape_end, 10.0));

	getXmlValue(tag::cross_type, mCross);
	
	mRadiusCorner = getXmlValue(tag::radius_corner, 0.0);

	//Сохраняем текущий карандаш, так как надо отрабатывать выделение коннеторов при подводе к ним мышки
	mPen = pen();
	
	
	//Значения этих переменных вынесены в переменные, потому что они интерактивно изменяются пользователем,
	//Требуется изменить XML перед сохранением, длясохранения действий пользователя
	//Непосредственное испрользование из XML требует затрат процессора на постоянное извлечение и перезапись,
	//нерационально
	//getXmlValue(tag::pos_coordinate, mPositionPoint );
	//mScaleX = getXmlValue(tag::kx, 1.0);
	getXmlValue(tag::structure_dot, mPoints );

	//3. Нарисуем
	draw();

}

void OptimaConnector::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
	if (mIsHighlight)
	{
		QPen localPen(mPen);
		localPen.setWidth(mPen.width() + 1);
		setPen(localPen);
	}
	else
	{
		setPen(mPen);
	}
	QGraphicsPathItem::paint(painter, option, widget);

	painter->setBrush(this->pen().color());
	painter->drawPath(mPathArrow);
}

void OptimaConnector::draw()
{
	QPainterPath pathLine;
	QPolygonF points;

	Q_ASSERT(mPoints.size() >= 2);

	// рисуем стрелку начала, и соотвественно меняем начальную точку отрисовки коннектора
	QPointF startPoint = mBeginArrow.getPath(mPathArrow, mPoints.at( 0 ), mPoints.at( 1 ));
	points.push_back( startPoint );

	for ( int i = 1; i < mPoints.size( ) - 1; ++i )
	{
		points.push_back( mPoints.at( i ) );
	}

	// рисуем конец
	const QPointF endPoint = mEndArrow.getPath( mPathArrow, mPoints.at( mPoints.size() - 1 ), mPoints.at( mPoints.size() - 2 ));

	points.push_back( endPoint);

	pathLine.moveTo( *points.begin() );

	for ( int i = 1; i < points.size(); ++i )
	{
		// учтем расстояние занимаемое элементом в начале отрезка, тэг ShapeBegin
		qreal lengthFromBegin = QLineF(mPoints.at(i - 1), points.at( i - 1 )).length();

		// точки пересечения с текущим отрезком храняться от начала отрезка
		const QLineF originalLine(points.at( i - 1 ), points.at( i ));
		mCross.draw( pathLine, originalLine,  mPoints.at(i - 1).getCrossingWithConnetorLengths(), lengthFromBegin );

		// точка рисования остановилась после последнего пересечения
		// дорисовываем до угла и сам угол
		drawCorner( pathLine, originalLine, i );
	}
	
	setPath( pathLine );

	//if ( m_drop_shadow )
	//{
	//	QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();	
	//	effect->setBlurRadius(10.);
	//	m_lines->setGraphicsEffect( effect );
	//}
	//else
	//{
	//	m_lines->setGraphicsEffect( 0 );
	//}	


	//m_arrows->setZValue( m_z_order );
	//m_arrows->setPath( pathArrow );

	//QBrush con_brush( m_color, Qt::SolidPattern );
	//m_arrows->setBrush( con_brush );
	//m_arrows->setPen( con_pen );
	//if ( m_drop_shadow )
	//{
	//	QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();	
	//	effect->setBlurRadius(10.);
	//	m_arrows->setGraphicsEffect( effect );
	//}
	//else
	//{
	//	m_arrows->setGraphicsEffect( 0 );
	//}	
	//m_scene->addItem( m_arrows.data() );

	//if ( is_selected( ) )
	//{
	//	redraw_coners( redraw_borders );
	//}
}

qreal OptimaConnector::getCircleRadius(const int indexCorner) const
{
	qreal radius( mRadiusCorner );

	const QLineF line1( mPoints.at( indexCorner - 1 ), mPoints.at( indexCorner ) );
	const QLineF line2( mPoints.at( indexCorner + 1), mPoints.at( indexCorner ) );

	qreal len = std::min( line1.length(), line2.length() );

	if ( len < radius * 2. )
	{
		radius = len / 2.;
	}

	return radius;

}

void OptimaConnector::drawCorner( QPainterPath &path, const QLineF originalLine, const int indexCorner ) const
{
	Q_ASSERT( mPoints.count() > indexCorner );

	const QLineF original_line1( mPoints.at( indexCorner - 1 ), mPoints.at( indexCorner ) );
	QLineF line1( original_line1 );

	// это конец отрезка или радиус не задан или это коннектор не angled 
	if ( mRadiusCorner == 0.0 || indexCorner + 1 >= mPoints.count() )
	{
		path.lineTo( originalLine.p2() );
		return;
	}

	QLineF line2( mPoints.at( indexCorner + 1 ), mPoints.at( indexCorner ) );

	// отсечем нулевые отрезки
	if ( line1.length() == 0 || line2.length() == 0 )
	{
		path.lineTo( original_line1.p2() );
		return;
	}

	// это последовательные прямые
	if ( line1.intersect( line2, NULL ) == QLineF::NoIntersection )
	{
		path.lineTo( original_line1.p2() );
		return;
	}

	qreal radius = getCircleRadius( indexCorner );

	line1.setLength( line1.length() - radius );
	line2.setLength( line2.length() - radius );

	qreal angle = line1.angleTo( QLineF( line2.p1(), line2.p2() ) );

	if ( angle == 0. )
	{
		path.lineTo( original_line1.p2() );
		return;
	}

	path.lineTo( line1.p2() );
	path.quadTo ( original_line1.p2(), line2.p2() );

}
