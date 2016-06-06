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

void OptimaText::draw()
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
		if (deltaWidth > 0 && deltaWidth < 5)
		{
			mTextWidth = textWidth();
		}
		setTextWidth(mTextWidth);
	}	
}
