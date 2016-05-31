#include "stdafx.h"
#include "tag.h"
#include "optimafigure.h"

OptimaFigure::OptimaFigure(const QString &itemUuid) : OptimaElement(this)
{
	setData(tag::data::uuid, itemUuid);

}

void OptimaFigure::apply(const QDomNode & figure)
{
	applyXml(figure);

	parseStructureDot(figure);

	applyPath();
}

void OptimaFigure::scale()
{
	QMatrix matrixScale;
	matrixScale = matrixScale.scale( 1.0, 1.0 );
	
	mPoints.clear();
	for( int i = 0; i < mOriginalPoints.size(); i++ ) {
		mPoints.push_back(OptimaPoint(matrixScale.map(mOriginalPoints[i]), mOriginalPoints[i].getRadius()));
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

void OptimaFigure::parseStructureDot(const QDomNode &figure)
{
	const QDomNodeList dots = getXmlNode( tag::structure_dot ).childNodes();
	if (dots.isEmpty())
	{
		return;
	}

	mOriginalPoints.clear();
	for ( int i = 0; i < dots.size( ); ++i )
	{
		int radius;

		mOriginalPoints << OptimaPoint(dots.at( i ));
	}
	mOriginalPoints << mOriginalPoints.first( );
	
	//mPoints = mOriginalPoints;
}

void OptimaFigure::applyPath()
{
	scale();
	
	QPainterPath path;

	for( int i = 1; i < mPoints.size(); i++ ) {
		int end_i = i + 1;

		if ( end_i >= mPoints.size() ) 
		{
			end_i = 1;
		}
		
		QLineF line1(createLine(i-1, i));
		QLineF line2(createLine(end_i, i));
		
		if ( i == 1 ) 
		{
			path.moveTo( line1.p2() );
		} 
		else 
		{
			path.lineTo( line1.p2() );
		}

		path.quadTo( mPoints[ i ], line2.p2() );
	}

	path.closeSubpath();

	setPath(path);
	setFlag(ItemIsSelectable);
	setFlag(ItemIsMovable);
}

QLineF OptimaFigure::createLine(int iStart, int iEnd) const
{
	QLineF line( mPoints[ iStart ], mPoints[ iEnd ] );
	int radius = mPoints[ iEnd ].getRadius();

	//≈сли длина стороны фигуры меньше чем два размера радиуса,
	//то просто начнем закругление с середины
	if ( line.length() > radius * 2 )
	{
		line.setLength( line.length() - radius );
	} else {
		line.setLength( line.length() / 2 );
	}

	return line;
}
