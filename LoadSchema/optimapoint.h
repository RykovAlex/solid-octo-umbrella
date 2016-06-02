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

	const OptimaPoint::OptimaLengthVector & getCrossingWithConnetorLengths() const;
	
	void addCrossingWithConnetorLength(qreal length);

	void clearCrossingWithConnetorLengths();

protected:

private:	
	int mRadius;///<������ ���������� � ���� �����

	OptimaLengthVector mCrossingWithConnectorLengths;///<������ ���������� �� ���� �����, �� ����� ����������� � ������� ������������

	///�������� �� ������ ���� x:y[:radius] ����� � ������ ����������, � ������������������� ��� ������
	void initialize( const QString & text );

};


