#pragma once

enum OptimaConnectorArrowShape
{
	  connector_arrow_no
	, connector_arrow_empty
	, connector_arrow_filled
	, connector_arrow_doubleempty
	, connector_arrow_doublefilled
	, connector_arrow_emptycircle
	, connector_arrow_filledcircle
	, connector_arrow_emptydiamond
	, connector_arrow_filleddiamond
	, connector_arrow_filledbox
	, connector_arrow_emptybox
	, connector_arrow_crow
	, connector_arrow_filledinv
	, connector_arrow_emptyinv
	, connector_arrow_tee
	, connector_arrow_vee
	, connector_arrow_curve
	, connector_arrow_ondef
};

class OptimaConnectorArrow
{
public:
	OptimaConnectorArrow( OptimaConnectorArrowShape _shape, QChar _mod, QChar _side);

	OptimaConnectorArrow( const QString &_shape, QChar _mod, QChar _side);

	OptimaConnectorArrow( OptimaConnectorArrowShape _shape);

	OptimaConnectorArrow( OptimaConnectorArrowShape _shape, bool isBegining);

	bool operator ==( OptimaConnectorArrowShape _shape );

	bool operator ==( const char *_shape_as_text );
	
	operator QString();

	bool isLikeGraphviz();
	
	bool isEmpty();
	
	bool isRight();
	
	bool isLeft();

	void apply(const QString &shapeName);

	qreal getSize() const;

	void setSize(qreal size);

	QPointF getPath( QPainterPath & path, const QPointF & beginPoint, const QPointF & endPoint );

protected:

private:
	QString mShapeAsText;

	OptimaConnectorArrowShape mShape;

	QChar mMod;

	QChar mSide;

	qreal mSize;

	bool mIsBegining;

	void applyAsGraphviz(const QString & shapeName);
	
	void getQuadPoints( const qreal arrowSize, const qreal arrowWidth,  const QLineF l0, QPointF &corner1, QPointF &corner2, QPointF &corner3, QPointF &corner4, QPointF &corner5 );
	
	void rotateArrow( QPolygonF &pp, const QPointF &at_point, const QPointF &to_point );

	QPointF drawArrowOndef(const QLineF &l0, QPainterPath &path);

	QPointF drawFilledCircle(const QLineF &l0, QPainterPath &path);

	QPointF drawEmptyCircle(const QLineF &l0, QPainterPath &path);

	QPointF drawDoubleEmptyArrow(const QLineF &l0, QLineF &l1, QLineF &l2, QPainterPath &path, const QPointF & endPoint);

	QPointF drawDoubleFilledArrow(const QLineF &l0, QLineF &l1, QLineF &l2, QPainterPath &path);

	QPointF drawEmptyArrow(const QLineF &l0, QLineF &l1, QLineF &l2, QPainterPath &path);

	QPointF drawFilledArrow(const QLineF &l0, QLineF &l1, QLineF &l2, QPainterPath &path);

	QPointF drawTriangleArrow(const QLineF &l0, QLineF &l1, QLineF &l2, QPainterPath &path);

	QPointF drawBoxArrow(const QLineF &l0, QPainterPath &path);

	QPointF drawCurveArrow(const QLineF &l0, QPainterPath &path);

	QPointF drawDiamondArrow(const QLineF &l0, QLineF &l1, QLineF &l2, QPainterPath &path);
};


