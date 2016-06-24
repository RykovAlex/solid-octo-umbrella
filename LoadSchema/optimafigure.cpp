#include "stdafx.h"
#include "tag.h"
#include "optimafigure.h"
#include "optimapath.h"

OptimaFigure::OptimaFigure(const QString &itemUuid, OptimaView *view) : OptimaElement(this, itemUuid, view)
{
	setData(tag::data::linkable, true);
	setFlag(ItemIsSelectable);
	setFlag(ItemIsMovable);
}

void OptimaFigure::apply()
{
	//1. �������� ������� ����������
	//��� ���������� �������� �� XML ������ ��� ��� ��������� ������� ������
	//��������� ��������� ��������, ������������ ����� ��������� ��������������� ����� ��������������
	//� �� �������. �������� ���������. �� ������� ��������� XML ��� ���������� ��������
	getXmlValue(tag::structure_dot, mOriginalPoints );
	mOriginalPoints << mOriginalPoints.first( );
	
	setCursor(QCursor( QPixmap( ":/images/resources/cursor_move_figure.png"), 0, 0));
	//�������� ���� ���� ���������� �������� � ����������, ������ ��� ��� ������������ ���������� �������������,
	//��������� �������� XML ����� �����������, ������������� �������� ������������
	//���������������� �������������� �� XML ������� ������ ���������� �� ���������� ���������� � ����������,
	//�������������
	getXmlValue(tag::pos_coordinate, mPositionPoint );
	mScaleX = getXmlValue(tag::kx, 1.0);
	mScaleY = getXmlValue(tag::ky, 1.0);

	//3. ��������� � ������ ������� �� �����
	setPos(mPositionPoint);
}

void OptimaFigure::scale()
{
	QMatrix matrixScale;
	matrixScale = matrixScale.scale( mScaleX, mScaleY );
	
	mPoints.clear();
	for( int i = 0; i < mOriginalPoints.size(); i++ ) {
		mPoints.push_back(OptimaPoint(matrixScale.map(mOriginalPoints.at(i)), mOriginalPoints.at(i).getRadius()));
	}
	//m_polygon->setPath( doPath( m_points ) );

	//QRectF boundingRect( m_polygon->boundingRect() );
	//QRectF adjustedBoundingRect( boundingRect.adjusted( -m_double_adjust, -m_double_adjust, m_double_adjust, m_double_adjust) );

	//QMatrix mx_scale2;
	//QPolygonF m_points_original_double;
	//QPolygonF m_points_double;
	//mx_scale2 = mx_scale2.scale( adjustedBoundingRect.width() / boundingRect.width(), adjustedBoundingRect.height() / boundingRect.height() );

	//if ( !m_points_original.empty() ) 
	//{
	//	m_points_double = mx_scale2.map( m_points );
	//}

	//m_polygon_double->setPath( doPath( m_points_double ) );

}

void OptimaFigure::draw(bool isProcessLoading /*= flase*/)
{
	scale();
	
	QPainterPath path;

	for( int i = 1; i < mPoints.size(); i++ ) {
		int end_i = i + 1;

		if ( end_i >= mPoints.size() ) 
		{
			end_i = 1;
		}
		
		QLineF line1(createLineToCurve(i-1, i));
		QLineF line2(createLineToCurve(end_i, i));
		
		if ( i == 1 ) 
		{
			path.moveTo( line1.p2() );
		} 
		else 
		{
			path.lineTo( line1.p2() );
		}

		path.quadTo( mPoints.at( i ), line2.p2() );
	}

	path.closeSubpath();

	setPath(path);
	
	splitOnPieces();
}

void OptimaFigure::splitOnPieces()
{
	shapePieces.clear();
	//QRectF itemSceneRect(mapRectToScene(boundingRect()));
	QRectF itemSceneRect((boundingRect()));

	QPointF centerPoint( QLineF(itemSceneRect.topLeft(), itemSceneRect.bottomRight()).pointAt(0.5));
	
	addShapePieces(centerPoint, QLineF(itemSceneRect.topLeft(), itemSceneRect.topRight()));
	addShapePieces(centerPoint, QLineF(itemSceneRect.topRight(), itemSceneRect.bottomRight()));
	addShapePieces(centerPoint, QLineF(itemSceneRect.bottomRight(), itemSceneRect.bottomLeft()));
	addShapePieces(centerPoint, QLineF(itemSceneRect.bottomLeft(), itemSceneRect.topLeft()));
}

void OptimaFigure::addShapePieces(const QPointF centerPoint, const QLineF &rectSegment)
{
	QPolygonF p;
	
	p << centerPoint << rectSegment.p1() << rectSegment.p2();
	
	shapePieces << p;
}

QLineF OptimaFigure::createLineToCurve(int iStart, int iEnd) const
{
	QLineF line( mPoints.at( iStart ), mPoints.at( iEnd ) );
	int radius = mPoints.at( iEnd ).getRadius();

	//���� ����� ������� ������ ������ ��� ��� ������� �������,
	//�� ������ ������ ����������� � ��������
	if ( line.length() > radius * 2 )
	{
		line.setLength( line.length() - radius );
	} else {
		line.setLength( line.length() / 2 );
	}

	return line;
}

void OptimaFigure::onMarkerMove(const OptimaBaseMarker* marker)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void OptimaFigure::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
	painter->setPen(pen());
	painter->setBrush(brush());
	painter->drawPath(path());
	if (mIndexShapePieces >= 0)
	{
		painter->setOpacity(0.25);
		painter->setPen(Qt::red);
		painter->drawRect(boundingRect());

		painter->setPen(Qt::NoPen);
		painter->setBrush(QBrush(Qt::gray));
		painter->setOpacity(0.25);
		//painter->setClipPath(path());
		painter->drawPolygon(shapePieces.at(mIndexShapePieces));
	}

}

void OptimaFigure::setHighlightShapePieces(const QPointF & scenePos, bool enabled)
{
	
	if (enabled)
	{
		QPointF itemPos = mapFromScene(scenePos);
		mIndexShapePieces = 0;
		foreach(const QPolygonF &p, shapePieces)
		{
			if (p.containsPoint(itemPos, Qt::OddEvenFill))
			{
				break;
			}
			++mIndexShapePieces;
		}
	}
	else
	{
		mIndexShapePieces = -1;
	}
	update();
}

