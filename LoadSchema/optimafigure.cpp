#include "stdafx.h"
#include "optimafigure.h"



OptimaFigure::OptimaFigure()
{
	QPainterPath path( QPointF(0,0));
	path.lineTo(0,100);
	path.lineTo(100,100);
	path.lineTo(100,0);
	path.lineTo(0,0);
	setPath( path );
}
