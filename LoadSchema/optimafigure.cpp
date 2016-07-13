#include "stdafx.h"
#include "tag.h"
#include "optimafigure.h"
#include "optimapath.h"
#include "optimaconnectorpathfinder.h"

OptimaFigure::OptimaFigure(const QString &itemUuid, OptimaView *view) : OptimaElement(this, itemUuid, view)
{
	setData(tag::data::linkable, true);
	setFlag(ItemIsSelectable);
	setFlag(ItemIsMovable);
}

void OptimaFigure::apply()
{
	//1. Заполним рабочие переменные
	//Эта переменная выделена из XML потому что при изменении размера фигуры
	//постоянно требуется пересчет, теоритически можно извлекать непосредственно перед использованием
	//а не хранить. Значение неизменно. не требует изменения XML при сохранении элемента
	getXmlValue(tag::structure_dot, mOriginalPoints );
	mOriginalPoints << mOriginalPoints.first( );
	
	setCursor(QCursor( QPixmap( ":/images/resources/cursor_move_figure.png"), 0, 0));
	//Значения этих трех переменных вынесены в переменные, потому что они интерактивно изменяются пользователем,
	//Требуется изменить XML перед сохранением, длясохранения действий пользователя
	//Непосредственное испрользование из XML требует затрат процессора на постоянное извлечение и перезапись,
	//нерационально
	getXmlValue(tag::pos_coordinate, mPositionPoint );
	mScaleX = getXmlValue(tag::kx, 1.0);
	mScaleY = getXmlValue(tag::ky, 1.0);

	//3. Переестим в нужную позицию на схеме
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

QLineF OptimaFigure::getEdgeLine(const QRectF & sceneBoundingRect,  OptimaLinkedRect::rectSide rectSide)
{
	switch (rectSide)
	{
	case OptimaLinkedRect::top:
		return QLineF(sceneBoundingRect.topLeft(), sceneBoundingRect.topRight());
	case OptimaLinkedRect::right:
		return QLineF(sceneBoundingRect.topRight(), sceneBoundingRect.bottomRight());
	case OptimaLinkedRect::bottom:
		return QLineF(sceneBoundingRect.bottomRight(), sceneBoundingRect.bottomLeft());
	case OptimaLinkedRect::left:
		return QLineF(sceneBoundingRect.bottomLeft(), sceneBoundingRect.topLeft());
	default:
		Q_ASSERT(false);
	}

	return QLineF();
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
	QRectF itemRect((boundingRect()));

	QPointF centerPoint( QLineF(itemRect.topLeft(), itemRect.bottomRight()).pointAt(0.5));
	
	addShapePieces(centerPoint, getEdgeLine(itemRect, OptimaLinkedRect::top));
	addShapePieces(centerPoint, getEdgeLine(itemRect, OptimaLinkedRect::right));
	addShapePieces(centerPoint, getEdgeLine(itemRect, OptimaLinkedRect::bottom));
	addShapePieces(centerPoint, getEdgeLine(itemRect, OptimaLinkedRect::left));
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

	//Если длина стороны фигуры меньше чем два размера радиуса,
	//то просто начнем закругление с середины
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

void OptimaFigure::setLinkedHighlight(bool enabled, const QPointF & scenePos /*= QPointF()*/)
{
	
	if (enabled)
	{
		checkLinkedHighlight(scenePos, mIndexShapePieces);
	}
	else
	{
		mIndexShapePieces = -1;
	}
	
	setData(tag::data::linkingParameter, mIndexShapePieces);
	
	update();
}

bool OptimaFigure::checkLinkedHighlight(const QPointF & scenePos, int & indexShapePieces)
{
	QPointF itemPos = mapFromScene(scenePos);
	indexShapePieces = 0;
	foreach(const QPolygonF &p, shapePieces)
	{
		QPolygonF p1(OptimaConnectorPathFinder::getMarkerRect(itemPos));

		//if (p.containsPoint(itemPos, Qt::OddEvenFill))
		if (!p.intersected(p1).isEmpty())
		{
			return true;
		}
		indexShapePieces++;
	}
	
	indexShapePieces = -1;

	return false;
}

bool OptimaFigure::checkLinkedHighlight(const QPointF & scenePos)
{
	int indexShapePieces;

	return checkLinkedHighlight(scenePos, indexShapePieces);
}

QPointF OptimaFigure::getIntersectPoint(const QLineF line) const
{
	for(int i = 0; i < mPoints.size() - 1; ++i)
	{
		QPointF intersectPoint;
		QLineF figureLine(mapToScene(mPoints.at(i)), mapToScene(mPoints.at(i+1)));

		if (figureLine.intersect(line, &intersectPoint) == QLineF::BoundedIntersection)
		{
			return intersectPoint;
		}		
	}

	return line.p1();
}

int OptimaFigure::type() const
{
	return Type;
}

OptimaLinkedRect::OptimaLinkedRect(const QRectF &rect, const rectSide side) : QRectF(rect)
	, mSide(side)
{

}

OptimaLinkedRect::rectSide OptimaLinkedRect::side() const
{
	return mSide;
}
