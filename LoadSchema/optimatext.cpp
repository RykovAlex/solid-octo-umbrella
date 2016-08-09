#include "stdafx.h"
#include "optimatext.h"
#include "tag.h"



OptimaText::OptimaText(const QString &itemUuid): OptimaElement(this, itemUuid)	
{

}

void OptimaText::apply()
{
	//1. �������� ������� ����������
	//��� �������� �� ���������� ������������� ������������, �������� � ���������� ��� �� ���������� 
	//� ������ ��������� ������� (������� draw)
	mHalign = getXmlValue(tag::halign, QString(""));
	setFont(getXmlValue(tag::name_font, QFont("Arial")));
	setDefaultTextColor(getXmlValue(tag::text_color, QColor(Qt::black)));	
	setRotation(getXmlValue(tag::angle, 0.0));

	setCursor(QCursor( QPixmap(":/images/resources/cursor_move_text.png"), 0, 0));

	//�������� ���� ���������� �������� � ����������, ������ ��� ��� ������������ ���������� �������������,
	//��������� �������� XML ����� �����������, ��� ���������� �������� ������������
	//���������������� �������������� �� XML ������� ������ ���������� �� ���������� ���������� � ����������,
	//�������������
	getXmlValue(tag::pos_coordinate, mPositionPoint );
	mText = getXmlValue(tag::text, QString(""));
	mTextWidth = getXmlValue(tag::text_width, 0.0);
	
	//2. ��������� � ������ ������� �� �����
	setPos(mPositionPoint);
}

void OptimaText::draw(bool isProcessLoading /*= flase*/)
{
	if (mHalign.isEmpty())
	{
		setPlainText(mText);
	}
	else
	{
		setHtml( "<p align="""+mHalign+""">" + mText + "</p>");
	}
	
	if ( mTextWidth == 0 )
	{
		setTextWidth( -1 );
		mTextWidth = boundingRect().width();
	} 
	else 
	{
		//������ �� ����� � Win7 ��� ��� �� ���������� ������ ������� ������ ��� Xp
		//��� ������ ������ ��������
		int deltaWidth = boundingRect().width() - mTextWidth;
		if (deltaWidth > 0 && deltaWidth < 3)
		{
			mTextWidth = textWidth();
		}
		setTextWidth(mTextWidth);
	}	
}

void OptimaText::onMarkerMove(const OptimaBaseMarker* marker)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void OptimaText::onHoverEnter(QGraphicsSceneHoverEvent *event)
{
	throw std::logic_error("The method or operation is not implemented.");
}

void OptimaText::onHoverLeave(QGraphicsSceneHoverEvent* hoverEvent)
{
	throw std::logic_error("The method or operation is not implemented.");
}

int OptimaText::type() const
{
	return Type;
}

void OptimaText::setLinkedHighlight(bool enabled, const QPointF & scenePos)
{
	//������ �������, ���� ������� �� ����� ����� �����������
	return;
}

bool OptimaText::checkLinkedHighlight(const QPointF & scenePos)
{
	throw std::logic_error("The method or operation is not implemented.");
}

QPointF OptimaText::getIntersectPoint(const QLineF line) const
{
	return QPointF();
}
