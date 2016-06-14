#include "stdafx.h"
#include "optimaconnector.h"
#include "tag.h"
#include "optimaconnectormovemarker.h"

OptimaConnector::OptimaConnector(const QString &itemUuid) 
	:OptimaElement(this, itemUuid)
	,mBeginArrow(connector_arrow_no, true)
	,mEndArrow(connector_arrow_no, false)
	,mIsHighlight(false)
{
	//��� QGraphicView ����� ����, ���� �� ��������� ������������ ����� ������, �� ��� �������� ���.
	//� ��� �������� ��� ����� ��������, ������� �����������, ��������� �� ������ ��� ����������� 
	//, ����� ������ � ����� OptimaView
	setAcceptedMouseButtons(Qt::NoButton);
}

void OptimaConnector::apply()
{
	//1. �������� ������� ����������
	//��� ���������� �������� �� XML ������ ��� ����������� ��� ���������
	//�������� ���������. �� ������� ��������� XML ��� ���������� ��������
	getXmlValue(tag::shape_begin, mBeginArrow);
	mBeginArrow.setSize(getXmlValue(tag::size_shape_begin, 10.0));

	getXmlValue(tag::shape_end, mEndArrow);
	mEndArrow.setSize(getXmlValue(tag::size_shape_end, 10.0));

	getXmlValue(tag::cross_type, mCross);
	
	mRadiusCorner = getXmlValue(tag::radius_corner, 0.0);
	
	//�������� ��� ���������
	mIsAngled�onnector = getXmlValue(tag::type, QString("direct")) == "rect";


	//��������� ������� ��������, ��� ��� ���� ������������ ��������� ���������� ��� ������� � ��� �����
	//��������� ��������� ��������� �������������� � applyXml
	mPen = pen();	
	
	//�������� ���� ���������� �������� � ����������, ������ ��� ��� ������������ ���������� �������������,
	//��������� �������� XML ����� �����������, ������������� �������� ������������
	//���������������� �������������� �� XML ������� ������ ���������� �� ���������� ���������� � ����������,
	//�������������
	getXmlValue(tag::structure_dot, mPoints );

	//���� ����� �������� ��������������� ���������, ������� � �������� ��������� ����� ���������� �� ���������� �����������
	buildPath(mCross);
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

void OptimaConnector::draw(bool isProcessLoading /*= false*/)
{	
	//��� ������ �������� �������� �� �������� ���� ����������� � ����������� ����� �� �����������
	//��� ����������� ����� ������� draw ��� ���, ����� ����������� ����� ����������� � ������� �����������
	if (isProcessLoading)
	{
		return;
	}
	
	setPath( mConnectorPath.toPath() );
	//if ( is_selected( ) )
	//{
	//	redraw_coners( redraw_borders );
	//}
}

void OptimaConnector::buildPath(const OptimaCross & cross)
{
	Q_ASSERT(mPoints.size() >= 2);

	//����� ����� ����������� ������� ������ � ����� ���������
	QPainterPath pathArrow;

	// ������ ������� ������, � ������������� ������ ��������� ����� ��������� ����������
	QPointF startPoint = mBeginArrow.getPath(pathArrow, mPoints.at( 0 ), mPoints.at( 1 ));

	mConnectorPath = OptimaPath(startPoint, cross);

	for ( int i = 1; i < mPoints.size( ) - 1; ++i )
	{
		mConnectorPath.lineTo(mPoints.at( i ), QLineF(mPoints.at( i + 1), mPoints.at( i )), mRadiusCorner);
	}

	// ������ �����
	const QPointF endPoint = mEndArrow.getPath( pathArrow, mPoints.at( mPoints.size() - 1 ), mPoints.at( mPoints.size() - 2 ));

	mConnectorPath.lineTo(endPoint);

	// �������� ��� ��������� � paint()
	mPathArrow = pathArrow;
}

void OptimaConnector::intersected(OptimaPath & connectorPath)
{
	mConnectorPath.intersected(connectorPath);
}

void OptimaConnector::moveLineEvent(const OptimaConnectorMoveMarker* moveMarker)
{
	throw std::logic_error("The method or operation is not implemented.");
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

		// ��������� ����� ��������� ����, �� � ����� ���������� �����
		// ��� ��� ��� ����������� ���� �� ����� ����� ������� ����, �� � item->mConnectorPath
		// ����� ����� ���� ���������� ����� ����������� � ������ ����� ���������
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

void OptimaConnector::markerMoveEvent(const OptimaBaseMarker* marker)
{
	const OptimaConnectorMoveMarker* moveMarker = dynamic_cast<const OptimaConnectorMoveMarker*>(marker);
	if (moveMarker != nullptr)
	{
		moveLineEvent(moveMarker);
	}
}

void OptimaConnector::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	//QGraphicsItem::mousePressEvent(event);
}

void OptimaConnector::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	setCursor(QCursor( QPixmap( mIsAngled�onnector ? ":/images/resources/cursor_move_rect.png" : ":/images/resources/cursor_move_direct.png" )));
	//setCursor(Qt::SizeAllCursor);
}

