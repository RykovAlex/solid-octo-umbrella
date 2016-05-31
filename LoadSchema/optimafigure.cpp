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
	
	mPoints = mOriginalPoints;
}

void OptimaFigure::applyPath()
{
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
