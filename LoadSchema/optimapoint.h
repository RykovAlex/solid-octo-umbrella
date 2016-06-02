#pragma once
#include <QPointF>
#include <QtXml/QDOMDocument>

class OptimaPoint : public QPointF
{
public:
	typedef QVector<qreal> OptimaLengthVector;
	
	OptimaPoint();
	OptimaPoint(qreal xpos, qreal ypos, int radius);
	OptimaPoint(const QPointF &point, int radius);
	OptimaPoint(QDomNode nodeDot);

	///принять изменения из переданного узла XML 
	void apply(const QDomNode &nodeDot);

	///getter для радиуса скругления
	int getRadius() const;

	const OptimaPoint::OptimaLengthVector & getCrossingWithConnetorLengths() const;
	
	void addCrossingWithConnetorLength(qreal length);

	void clearCrossingWithConnetorLengths();

protected:

private:	
	int mRadius;///<Радиус скругления в этой точке

	OptimaLengthVector mCrossingWithConnectorLengths;///<Массив расстояний от этой точки, до точек пересечения с другими коннекторами

	///Получить из строки вида x:y[:radius] точку и радиус скругления, и проинициализировать ими объект
	void initialize( const QString & text );

};


