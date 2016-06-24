#include "stdafx.h"
#include "optimaconnectorarrow.h"

OptimaConnectorArrow::OptimaConnectorArrow(OptimaConnectorArrowShape _shape, QChar _mod, QChar _side) 
	: mShape( _shape )
	, mMod( _mod )
	, mSide( _side )
	, mIsBegining(false)
	, mSize(10.0)
{
}

OptimaConnectorArrow::OptimaConnectorArrow(const QString &_shape, QChar _mod, QChar _side) 
	: mShapeAsText( _shape )
	, mMod( _mod )
	, mSide( _side )
	, mIsBegining(false)
	, mSize(10.0)
{
}

OptimaConnectorArrow::OptimaConnectorArrow(OptimaConnectorArrowShape _shape) 
	: mShape( _shape )
	, mMod( '\0' )
	, mSide( '\0' )
	, mIsBegining(false)
	, mSize(10.0)
{
}

OptimaConnectorArrow::OptimaConnectorArrow(OptimaConnectorArrowShape _shape, bool isBegining) 
	: mShape( _shape )
	, mMod( '\0' )
	, mSide( '\0' )
	, mIsBegining(isBegining)
	, mSize(10.0)
{
}

bool OptimaConnectorArrow::operator==(const char *_shape_as_text)
{
	return mShapeAsText == _shape_as_text;
}

bool OptimaConnectorArrow::operator==(OptimaConnectorArrowShape _shape)
{
	return mShape == _shape;
}

OptimaConnectorArrow::operator QString()
{
	QString ret( (mMod.isNull() ? "" : QString( mMod )) + (mSide.isNull() ? "" : QString( mSide )) + mShapeAsText);
	return ret;
}

bool OptimaConnectorArrow::isLikeGraphviz()
{
	return !mShapeAsText.isNull();
}

bool OptimaConnectorArrow::isEmpty()
{
	return mMod == QChar('o');
}

bool OptimaConnectorArrow::isRight()
{
	return mSide == QChar('r') || mSide == 0;
}

bool OptimaConnectorArrow::isLeft()
{
	return mSide == QChar('l') || mSide == 0;
}

void OptimaConnectorArrow::apply(const QString &shapeName)
{
	mMod = '\0';
	mSide = '\0';
	mShape = connector_arrow_no;

	if ( shapeName == "EmptyArrow" )
		mShape = connector_arrow_empty;
	else if ( shapeName == "FilledArrow" )
		mShape = connector_arrow_filled;
	else if ( shapeName == "DoubleEmptyArrow" )
		mShape = connector_arrow_doubleempty;
	else if ( shapeName == "DoubleFilledArrow" )
		mShape = connector_arrow_doublefilled;
	else if ( shapeName == "EmptyCircleArrow" )
		mShape = connector_arrow_emptycircle;
	else if ( shapeName == "FilledCircleArrow" )
		mShape = connector_arrow_filledcircle;
	else if ( shapeName == "FilledDiamondArrow" )
		mShape = connector_arrow_filleddiamond;
	else if ( shapeName == "EmptyDiamondArrow" )		
		mShape = connector_arrow_emptydiamond;
	else if ( shapeName == "FilledBoxArrow" )
		mShape = connector_arrow_filledbox;
	else if ( shapeName == "EmptyBoxArrow" )		
		mShape = connector_arrow_emptybox;
	else if ( shapeName == "CrowArrow" )
		mShape = connector_arrow_crow;
	else if ( shapeName == "FilledInvArrow" )
		mShape = connector_arrow_filledinv;
	else if ( shapeName == "EmptyInvArrow" )
		mShape = connector_arrow_emptyinv;
	else if ( shapeName == "TeeArrow" )
		mShape = connector_arrow_tee;
	else if ( shapeName == "VeeArrow" )
		mShape = connector_arrow_vee;
	else if ( shapeName == "CurveArrow" )
		mShape = connector_arrow_curve;
	else if ( shapeName == "OndefArrow" )
		mShape = connector_arrow_ondef;
	else
		applyAsGraphviz( shapeName );
}

qreal OptimaConnectorArrow::getSize() const
{
	return mSize;
}

void OptimaConnectorArrow::setSize(qreal size)
{
	if ( size <= 0 )
	{
		mSize = 0.0;
	}
	else
	if ( size > 20. )
	{
		mSize = 20.0;
	}
	else
		mSize = size;
}

void OptimaConnectorArrow::applyAsGraphviz(const QString & shapeName)
{
	const char* shapes[] = {
		"box" 
		, "crow" 
		, "curve" 
		, "diamond" 
		, "dot" 
		, "inv" 
		, "none" 
		, "normal" 
		, "tee" 
		, "vee" 
		, "ndef" 
	};
	QString s( shapeName );

	if ( s[0] == QChar('o') )
	{
		mMod = s[0];
		s.remove( 0, 1 );
	}
	if ( s[0] == QChar('l') || s[0] == QChar('r') )
	{
		mSide = s[0];
		s.remove( 0, 1 );
	}
	for( int i = 0; i < sizeof(shapes) / sizeof(shapes[0]); i++ )
	{
		QString shape( shapes[ i ] );
		if ( shape == s.left( shape.size() ) )
		{
			s.remove( 0, shape.size() );

			mShapeAsText = shape;
			break;
		}
	}
}

void OptimaConnectorArrow::getQuadPoints( const qreal arrowSize, const qreal arrowWidth,  const QLineF l0, QPointF &corner1, QPointF &corner2, QPointF &corner3, QPointF &corner4, QPointF &corner5 )
{
	const qreal diamond_angle = 90;
	QLineF l1( l0 ), l2( l0 );

	l1.setAngle( l0.angle( ) + diamond_angle );
	l2.setAngle( l0.angle( ) - diamond_angle );

	// рисуем квадрат 
	corner3 = l0.pointAt( arrowWidth / l0.length() );

	QLineF l3( corner3, l0.p1() ), l4( l3 );

	l3.setAngle( l3.angle( ) + diamond_angle );
	l4.setAngle( l4.angle( ) - diamond_angle );

	if ( mIsBegining )
	{
		corner1 = l1.pointAt( arrowSize / 2 / l1.length() );
		corner2 = l2.pointAt( arrowSize / 2 / l2.length() );

		corner4 = l3.pointAt( arrowSize / 2 / l3.length() );
		corner5 = l4.pointAt( arrowSize / 2 / l4.length() );
	}
	else
	{
		corner2 = l1.pointAt( arrowSize / 2 / l1.length() );
		corner1 = l2.pointAt( arrowSize / 2 / l2.length() );

		corner5 = l3.pointAt( arrowSize / 2 / l3.length() );
		corner4 = l4.pointAt( arrowSize / 2 / l4.length() );
	}
}

void OptimaConnectorArrow::rotateArrow( QPolygonF &poligon, const QPointF &atPoint, const QPointF &toPoint )
{
	QMatrix matrix;
	const qreal angle = 180.;
	// порядок матриц обратный проделываемым действиям
	matrix.translate( toPoint.x(), toPoint.y() );	
	matrix.rotate( angle );
	matrix.translate( -atPoint.x(), -atPoint.y() );

	poligon = matrix.map( poligon );
}

QPointF OptimaConnectorArrow::getPath( QPainterPath & path, const QPointF & beginPoint, const QPointF & endPoint )
{
	if ( *this == connector_arrow_no && !this->isLikeGraphviz() )
		return beginPoint;
	if ( *this == "none" )
		return beginPoint;
	if ( mSize == 0 )
		return beginPoint;

	const QLineF l0( beginPoint, endPoint );
	QLineF l1( l0 ), l2( l0 );

	// если короткое, то не рисуем стрелку
	if ( l0.length() < 1. )
	{
		return l0.p1();
	}

	l1.setAngle( l1.angle( ) + 30 );
	l2.setAngle( l2.angle( ) - 30 );
	

	if ( *this == connector_arrow_filleddiamond 
		|| *this == connector_arrow_emptydiamond
		|| *this == "diamond"
		)
	{
		// рисуем ромб
		return drawDiamondArrow(l0, l1, l2, path);
	} 
	else 
	if ( *this == "curve" )
	{
		// рисуем кривую
		return drawCurveArrow(l0, path);
	} 
	else 
	if ( *this == "box" || *this == "tee" )
	{
		// рисуем квадрат и треть квадрата
		return drawBoxArrow(l0, path);
	} 
	else 
	if ( *this == "inv" || *this == "normal" || ( *this == "vee" || *this == "crow" ) )
	{		
		// рисуем прямой и обратный треугольник
		return drawTriangleArrow(l0, l1, l2, path);
	}
	else 
	if ( *this == connector_arrow_filled )
	{		
		return drawFilledArrow(l0, l1, l2, path);

	}
	else
	if ( *this == connector_arrow_empty )
	{
		return drawEmptyArrow(l0, l1, l2, path);

	}
	else
	if ( *this == connector_arrow_doublefilled )
	{
		return drawDoubleFilledArrow(l0, l1, l2, path);
	}
	else
	if ( *this == connector_arrow_doubleempty )
	{
		return drawDoubleEmptyArrow(l0, l1, l2, path, endPoint);
	}
	else
	if ( (*this == "dot" && this->isEmpty()) || *this == connector_arrow_emptycircle )
	{
		return drawEmptyCircle(l0, path);
	}
	else
	if ( *this == "dot" || *this == connector_arrow_filledcircle )
	{
		return drawFilledCircle(l0, path);
	}
	else 
	if ( *this == "ndef" || *this == connector_arrow_ondef )
	{
		return drawArrowOndef(l0, path);
	}
	
	return l0.p1();
}

QPointF OptimaConnectorArrow::drawArrowOndef(const QLineF &l0, QPainterPath &path)
{
	const QPointF center = l0.pointAt( ( mSize / 2. ) / l0.length() );
	QLineF l3( center, l0.p2() ), l4( l3 );

	l3.setAngle( l0.angle( ) - 70 );
	l4.setAngle( l0.angle( ) - 250 );

	const QPointF corner1 = l3.pointAt( mSize / l3.length() );
	const QPointF corner2 = l4.pointAt( mSize / l4.length() );

	path.moveTo(corner1);
	path.lineTo(corner2);

	return l0.p1();
}

QPointF OptimaConnectorArrow::drawFilledCircle(const QLineF &l0, QPainterPath &path)
{
	const QPointF center = l0.pointAt( ( mSize / 2. ) / l0.length() );
	path.addEllipse( center, mSize / 2., mSize / 2. );

	return l0.pointAt( mSize / l0.length() );
}

QPointF OptimaConnectorArrow::drawEmptyCircle(const QLineF &l0, QPainterPath &path)
{
	const QPointF center = l0.pointAt( ( mSize / 2. ) / l0.length() );
	path.addEllipse( center, mSize / 2., mSize / 2. );
	path.addEllipse( center, mSize / 2., mSize / 2. );

	return l0.pointAt( mSize / l0.length() );
}

QPointF OptimaConnectorArrow::drawDoubleEmptyArrow(const QLineF &l0, QLineF &l1, QLineF &l2, QPainterPath &path, const QPointF & endPoint)
{
	QPolygonF pp;

	pp.push_back( l0.p1() );
	const QPointF corner1 = l1.pointAt( mSize / l1.length() );
	pp.push_back( corner1 );
	const QPointF corner2 = l2.pointAt( mSize / l2.length() );
	pp.push_back( corner2 );
	pp.push_back( l0.p1() );
	pp.push_back( corner2 );
	pp.push_back( corner1 );
	pp.push_back( l0.p1() );
	path.addPolygon( pp );

	QPolygonF p2;
	QPointF endFirst;
	l0.intersect( QLineF( corner1, corner2 ), &endFirst );

	QLineF l3( endFirst, endPoint ), l4( endFirst, endPoint );
	l3.setAngle( l3.angle( ) + 30 );
	l4.setAngle( l4.angle( ) - 30 );

	p2.push_back( endFirst );
	const QPointF corner3 = l3.pointAt( mSize / l3.length() );
	p2.push_back( corner3 );
	const QPointF corner4 = l4.pointAt( mSize / l4.length() );
	p2.push_back( corner4 );
	p2.push_back( endFirst );
	p2.push_back( corner4 );
	p2.push_back( corner3 );
	p2.push_back( endFirst );
	path.addPolygon( p2 );

	QPointF ret;
	l0.intersect( QLineF( corner3, corner4 ), &ret );
	return ret;
}

QPointF OptimaConnectorArrow::drawDoubleFilledArrow(const QLineF &l0, QLineF &l1, QLineF &l2, QPainterPath &path)
{
	QPolygonF pp;

	pp.push_back( l0.p1() );
	const QPointF corner1 = l1.pointAt( mSize / l1.length() );
	pp.push_back( corner1 );
	const QPointF corner2 = l2.pointAt( mSize / l2.length() );
	pp.push_back( corner2 );
	pp.push_back( l0.p1() );
	path.addPolygon( pp );

	QPolygonF p2;

	QPointF endFirst;
	l0.intersect( QLineF( corner1, corner2 ), &endFirst );
	QLineF l3( endFirst, l0.p2() ), l4( endFirst, l0.p2() );
	l3.setAngle( l3.angle( ) + 30 );
	l4.setAngle( l4.angle( ) - 30 );

	p2.push_back( endFirst );
	const QPointF corner3 = l3.pointAt( mSize / l3.length() );
	p2.push_back( corner3 );
	const QPointF corner4 = l4.pointAt( mSize / l4.length() );
	p2.push_back( corner4 );
	p2.push_back( endFirst );
	path.addPolygon( p2 );

	QPointF ret;
	l0.intersect( QLineF( corner3, corner4 ), &ret );
	return ret;
}

QPointF OptimaConnectorArrow::drawEmptyArrow(const QLineF &l0, QLineF &l1, QLineF &l2, QPainterPath &path)
{
	QPolygonF pp;

	pp.push_back( l0.p1() );
	const QPointF corner1 = l1.pointAt( mSize / l1.length() );
	pp.push_back( corner1 );
	const QPointF corner2 = l2.pointAt( mSize / l2.length() );
	pp.push_back( corner2 );

	pp.push_back( l0.p1() );
	pp.push_back( corner2 );
	pp.push_back( corner1 );

	pp.push_back( l0.p1() );
	path.addPolygon( pp );
	QPointF ret;
	l0.intersect( QLineF( corner1, corner2 ), &ret );
	return ret;
}

QPointF OptimaConnectorArrow::drawFilledArrow(const QLineF &l0, QLineF &l1, QLineF &l2, QPainterPath &path)
{
	QPolygonF pp;

	pp.push_back( l0.p1() );
	const QPointF corner1 = l1.pointAt( mSize / l1.length() );
	pp.push_back( corner1 );
	const QPointF corner2 = l2.pointAt( mSize / l2.length() );
	pp.push_back( corner2 );
	
	pp.push_back( l0.p1() );
	path.addPolygon( pp );
	QPointF ret;
	l0.intersect( QLineF( corner1, corner2 ), &ret );

	return ret;
}

QPointF OptimaConnectorArrow::drawTriangleArrow(const QLineF &l0, QLineF &l1, QLineF &l2, QPainterPath &path)
{
	QPolygonF pp;
	QPointF corner1, corner2;

	if ( mIsBegining )
	{
		corner1 = l1.pointAt( mSize / l1.length() );
		corner2 = l2.pointAt( mSize / l2.length() );
	}
	else 
	{
		corner2 = l1.pointAt( mSize / l1.length() );
		corner1 = l2.pointAt( mSize / l2.length() );
	}

	QPointF ret;

	if ( *this == "vee" || *this == "crow" )
		ret = l0.pointAt( mSize / 2.5 / l0.length() );
	else
		l0.intersect( QLineF( corner1, corner2 ), &ret );

	pp.push_back( l0.p1() );
	if ( this->isLeft() )
		pp.push_back( corner1 );
	pp.push_back( ret );
	if ( this->isRight() )
		pp.push_back( corner2 );
	pp.push_back( l0.p1() );

	if ( this->isEmpty() )
	{
		if ( this->isRight() )
			pp.push_back( corner2 );
		pp.push_back( ret );
		if ( this->isLeft() )
			pp.push_back( corner1 );
		pp.push_back( l0.p1() );
	}

	if ( *this == "crow" )
		l0.intersect( QLineF( corner1, corner2 ), &ret );

	if ( *this == "inv" || *this == "crow" )
	{
		rotateArrow( pp, l0.p1(), ret);
	}

	path.addPolygon( pp );

	if ( *this == "crow" )
		ret =  l0.p1();

	return ret;
}

QPointF OptimaConnectorArrow::drawBoxArrow(const QLineF &l0, QPainterPath &path)
{
	QPolygonF pp;
	QPointF corner1, corner2, corner3, corner4, corner5;

	getQuadPoints( mSize, ((*this == "tee") ? mSize / 3 : mSize), l0, corner1, corner2, corner3, corner4, corner5);

	pp.push_back( l0.p1() );

	if ( this->isRight() )
	{
		pp.push_back( corner2 );
		pp.push_back( corner4 );
	}

	pp.push_back( corner3 );

	if ( this->isLeft() )
	{
		pp.push_back( corner5 );
		pp.push_back( corner1 );
	}

	pp.push_back( l0.p1() );

	if ( this->isEmpty() )
	{
		if ( this->isLeft() )
		{
			pp.push_back( corner1 );
			pp.push_back( corner5 );
		}
		pp.push_back( corner3 );

		if ( this->isRight() )
		{
			pp.push_back( corner4 );
			pp.push_back( corner2 );
		}
		pp.push_back( l0.p1() );

	}
	path.addPolygon( pp );

	return corner3;
}

QPointF OptimaConnectorArrow::drawCurveArrow(const QLineF &l0, QPainterPath &path)
{
	QPointF corner1, corner2, corner3, corner4, corner5;

	getQuadPoints( mSize, mSize / 2, l0, corner1, corner2, corner3, corner4, corner5);

	if ( this->isRight() )
	{
		path.moveTo( l0.p1() );
		path.quadTo( corner2, corner4 ); 
		path.quadTo( corner2, l0.p1() ); 
	}
	if ( this->isLeft() )
	{
		path.moveTo( l0.p1() );
		path.quadTo( corner1, corner5 ); 
		path.quadTo( corner1, l0.p1() ); 
	}

	return l0.p1();
}

QPointF OptimaConnectorArrow::drawDiamondArrow(const QLineF &l0, QLineF &l1, QLineF &l2, QPainterPath &path)
{
	QPolygonF pp;
	const qreal diamond_angle = 30;

	l1.setAngle( l0.angle( ) + diamond_angle );
	l2.setAngle( l0.angle( ) - diamond_angle );

	// рисуем ромб
	pp.push_back( l0.p1() );

	QPointF corner1, corner2;

	const QPointF corner3 = l0.pointAt( mSize / l0.length() );
	QLineF l3( corner3, l0.p1() ), l4( l3 );

	l3.setAngle( l3.angle( ) + diamond_angle );
	l4.setAngle( l4.angle( ) - diamond_angle );

	l3.intersect( l2, &corner2 );
	l4.intersect( l1, &corner1 );

	if ( mIsBegining )
	{
		l3.intersect( l2, &corner1 );
		l4.intersect( l1, &corner2 );
	}
	else
	{
		l3.intersect( l2, &corner2 );
		l4.intersect( l1, &corner1 );
	}

	if ( this->isRight() )
		pp.push_back( corner1 );

	pp.push_back( corner3 );
	if ( this->isLeft() )
		pp.push_back( corner2 );
	pp.push_back( l0.p1() );

	if ( *this == connector_arrow_emptydiamond || this->isEmpty() )
	{
		if ( this->isLeft() )
			pp.push_back( corner2 );
		pp.push_back( corner3 );
		if ( this->isRight() )
			pp.push_back( corner1 );
		pp.push_back( l0.p1() );

	}

	path.addPolygon( pp );

	return corner3;
}
