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

	///������� ��������� �� ����������� ���� XML 
	void apply(const QDomNode &nodeDot);

	///getter ��� ������� ����������
	int getRadius() const;

	QString uuid() const { return mUuid; }

	void setUuid(QString val) { mUuid = val; }

	static const OptimaPointVector createVector(QPointF p1, QPointF p2);
protected:


private:	
	int mRadius;///<������ ���������� � ���� �����

	///�������� �� ������ ���� x:y[:radius] ����� � ������ ����������, � ������������������� ��� ������
	void initialize( const QString & text );

private:
	QString mUuid;
};



