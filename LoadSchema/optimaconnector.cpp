#include "stdafx.h"
#include "optimaconnector.h"
#include "tag.h"

OptimaConnector::OptimaConnector(const QString &itemUuid) 
	:OptimaElement(this, itemUuid)
	,mBeginArrow(connector_arrow_no)
{
}

void OptimaConnector::apply(const QDomNode & connector)
{
	//1. ������ XML
	applyXml(connector);

	//2. �������� ������� ����������
	//��� ���������� �������� �� XML ������ ��� ����������� ��� ���������
	//�������� ���������. �� ������� ��������� XML ��� ���������� ��������
	getXmlValue(tag::structure_dot, mPoints );
	getXmlValue(tag::shape_begin, mBeginArrow);
	mBeginArrow.setSize(getXmlValue(tag::size_shape_begin, 10.0));
	getXmlValue(tag::shape_begin, mEndArrow);
	mBeginArrow.setSize(getXmlValue(tag::size_shape_end, 10.0));

	//�������� ���� ���������� �������� � ����������, ������ ��� ��� ������������ ���������� �������������,
	//��������� �������� XML ����� �����������, ������������� �������� ������������
	//���������������� �������������� �� XML ������� ������ ���������� �� ���������� ���������� � ����������,
	//�������������
	//getXmlValue(tag::pos_coordinate, mPositionPoint );
	//mScaleX = getXmlValue(tag::kx, 1.0);
	
	//3. ��������
	draw();

	//4. ��������� � ������ ������� �� �����
	//setPos(mPositionPoint);
}

void OptimaConnector::draw()
{
	QPainterPath pathLine, pathArrow;
	QPolygonF points;

	// ������ ������� ������, � ������������� ������ ��������� ����� ��������� ����������
	QPointF startPoint = drawArrow( pathArrow, mPoints.at( 0 ), mPoints.at( 1 ), mBeginArrow, true );
	points.push_back( startPoint );

	for ( int i = 1; i < mPoints.size( ) - 1; ++i )
	{
		points.push_back( mPoints.at( i ) );
	}

	// ������ �����
	const QPointF & endPont = drawArrow( pathArrow, mPoints.at( mPoints.size() - 1 ), mPoints.at( mPoints.size() - 2 ), mEndArrow, false );
	points.push_back( endPont);

	pathLine.moveTo( *points.begin() );

	for ( int i = 1; i < points.size(); ++i )
	{
		// ����� ���������� ���������� ��������� � ������ �������, ��� ShapeBegin
		qreal from_begin = QLineF( mPoints.at(i - 1), points.at( i - 1 ) ).length();

		// ����� ����������� � ������� �������� ��������� �� ������ �������
		QLineF original_line( points.at( i - 1 ), points.at( i ) );
		draw_intersection( pathLine, original_line,  mPoints[ i - 1 ].m_intersect_point, from_begin );

		// ����� ��������� ������������ ����� ���������� �����������
		// ������������ �� ���� � ��� ����
		draw_corner( pathLine, original_line, i );
	}

	//if (!m_lines)
	//{
	//	m_lines = new QGraphicsPathItem( );
	//}

	m_lines->setZValue( m_z_order );
	m_lines->setPath( pathLine );
	m_lines->setBrush( Qt::NoBrush );

	QPen con_pen( m_color, m_width + (m_highlight?1:0), m_line_style );

	con_pen.setCosmetic( true );
	m_lines->setPen( con_pen );
	if ( m_drop_shadow )
	{
		QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();	
		effect->setBlurRadius(10.);
		m_lines->setGraphicsEffect( effect );
	}
	else
	{
		m_lines->setGraphicsEffect( 0 );
	}	
	m_scene->addItem( m_lines.data() );

	//if (!m_arrows)
	//{
	//	m_arrows = new QGraphicsPathItem( );
	//}

	m_arrows->setZValue( m_z_order );
	m_arrows->setPath( pathArrow );

	QBrush con_brush( m_color, Qt::SolidPattern );
	m_arrows->setBrush( con_brush );
	m_arrows->setPen( con_pen );
	if ( m_drop_shadow )
	{
		QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();	
		effect->setBlurRadius(10.);
		m_arrows->setGraphicsEffect( effect );
	}
	else
	{
		m_arrows->setGraphicsEffect( 0 );
	}	
	m_scene->addItem( m_arrows.data() );

	if ( is_selected( ) )
	{
		redraw_coners( redraw_borders );
	}
}

const QT_NAMESPACE::QPointF & OptimaConnector::drawArrow(QPainterPath pp_arrows, UnknownType param2, UnknownType param3, UnknownType m_begin_arrow, bool param5, UnknownType m_size_shape_begin)
{
	throw std::logic_error("The method or operation is not implemented.");
}
