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

	///������� ��������� �� ����������� ���� XML 
	void apply(const QDomNode &nodeDot);

	///getter ��� ������� ����������
	int getRadius() const;

protected:

private:	
	int mRadius;///<������ ���������� � ���� �����

	///�������� �� ������ ���� x:y[:radius] ����� � ������ ����������, � ������������������� ��� ������
	void initialize( const QString & text );
};


