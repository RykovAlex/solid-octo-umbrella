#include "stdafx.h"
#include "optimatext.h"
#include "tag.h"



OptimaText::OptimaText(const QString &itemUuid): OptimaElement(this, itemUuid)	
{

}

void OptimaText::apply()
{
	//1. Заполним рабочие переменные
	//Эти значения не изменяются пользователем интерактивно, вынесены в переменные так ка учавствуют 
	//в логике отрисовки объекта (функция draw)
	mHalign = getXmlValue(tag::halign, QString(""));
	setFont(getXmlValue(tag::name_font, QFont("Arial")));
	setDefaultTextColor(getXmlValue(tag::text_color, QColor(Qt::black)));	
	setRotation(getXmlValue(tag::angle, 0.0));

	//Значения этих переменных вынесены в переменные, потому что они интерактивно изменяются пользователем,
	//Требуется изменить XML перед сохранением, для сохранения действий пользователя
	//Непосредственное испрользование из XML требует затрат процессора на постоянное извлечение и перезапись,
	//нерационально
	getXmlValue(tag::pos_coordinate, mPositionPoint );
	mText = getXmlValue(tag::text, QString(""));
	mTextWidth = getXmlValue(tag::text_width, 0.0);
	
	//2. Переестим в нужную позицию на схеме
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
		//Почему то текст в Win7 при тех же настройках шрифта немного больше чем Xp
		//это должно решить проблему
		int deltaWidth = boundingRect().width() - mTextWidth;
		if (deltaWidth > 0 && deltaWidth < 5)
		{
			mTextWidth = textWidth();
		}
		setTextWidth(mTextWidth);
	}	
}
