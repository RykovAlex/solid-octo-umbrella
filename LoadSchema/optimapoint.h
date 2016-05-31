#pragma once
#include <QPointF>
#include <QtXml/QDOMDocument>

class OptimaPoint : public QPointF
{
public:
	OptimaPoint();
	OptimaPoint(qreal xpos, qreal ypos, int radius);
	OptimaPoint(const QPointF &point, int radius);
	OptimaPoint(QDomNode nodeDot);

	int getRadius() const;

	operator QPointF()
	{
		return (*this);
	}

	OptimaPoint operator= (const QPointF &point) 
	{
		return OptimaPoint(point.x(), point.y(), mRadius);
	}

protected:
private:
	int mRadius;

	///Получить из строки вида x:y[:radius] точку и радиус скругления, и проинициализировать ими объект
	void initialize( const QString & text );
};


