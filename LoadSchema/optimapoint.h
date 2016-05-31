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

	///�������� �� ������ ���� x:y[:radius] ����� � ������ ����������, � ������������������� ��� ������
	void initialize( const QString & text );
};


