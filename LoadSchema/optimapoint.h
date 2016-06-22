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

	///������� ��������� �� ����������� ���� XML 
	void apply(const QDomNode &nodeDot);

	///getter ��� ������� ����������
	int getRadius() const;

	QString uuid() const { return mUuid; }

	void setUuid(QString val) { mUuid = val; }

protected:


private:	
	int mRadius;///<������ ���������� � ���� �����

	///�������� �� ������ ���� x:y[:radius] ����� � ������ ����������, � ������������������� ��� ������
	void initialize( const QString & text );

private:
	QString mUuid;
};

typedef QVector<OptimaPoint> OptimaPointVector;

