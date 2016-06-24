#pragma once
#include <QPointF>
#include <QtXml/QDOMDocument>

class OptimaPoint;
typedef QVector<qreal> OptimaLengthVector;

typedef QVector<OptimaPoint> OptimaPointVector;

class OptimaPoint : public QPointF
{
public:

	OptimaPoint();
	OptimaPoint(qreal xpos, qreal ypos, int radius);
	OptimaPoint(const QPointF &point, int radius);
	OptimaPoint(QDomNode nodeDot);
	OptimaPoint(const QPointF &point);

	///принять изменения из переданного узла XML 
	void apply(const QDomNode &nodeDot);

	///getter для радиуса скругления
	int getRadius() const;

	QString uuid() const { return mUuid; }

	void setUuid(QString val) { mUuid = val; }

	static const OptimaPointVector createVector(QPointF p1, QPointF p2);
protected:


private:	
	int mRadius;///<Радиус скругления в этой точке

	///Получить из строки вида x:y[:radius] точку и радиус скругления, и проинициализировать ими объект
	void initialize( const QString & text );

private:
	QString mUuid;
};



