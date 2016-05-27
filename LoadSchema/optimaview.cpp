#include "stdafx.h"
#include "tag.h"
#include "optimaview.h"
#include "OptimaElement.h"
#include "optimafigure.h"

OptimaView::OptimaView(QWidget *parent) : QGraphicsView(parent) 
{ 
	setScene( new QGraphicsScene(parent) );
};


void OptimaView::loadFigures(const QDomNodeList &figures, bool loadAllways)
{
	for ( int nn = 0; nn < figures.size( ); ++nn )
	{
		const QDomNode &figure = figures.at( nn );
		const QString uuidFigure = figure.namedItem( tag::id ).toElement().text();
	
		if (uuidFigure.isEmpty())
		{			
			if ( !loadAllways )
				throw QString().append( tr( "У фигуры не найден UUID" ) );
			continue;
		}

		//Найдем на схеме элемент с uuid из xml
		const QList<QGraphicsItem*> itemList = items();
		for (QList<QGraphicsItem*>::const_iterator i = itemList.constBegin(); i != itemList.constEnd(); ++i )
		{
			QGraphicsItem* item = *i;
			OptimaElement *optimaElement = dynamic_cast<OptimaElement*>(item);
		}
		OptimaFigure *optimaFigure = new OptimaFigure(uuidFigure);
		scene()->addItem(optimaFigure);
	}
}

QString OptimaView::LoadScheme(const QString &filename, bool load_allways)
{
	int xml_error_column;
	int xml_error_line;
	QString xml_error_msg;
	
	//setScene( new QGraphicsScene );
	
	beforeExecute1CCall();
	
	QDomDocument doc;	
	try
	{
		if ( !doc.setContent( filename, &xml_error_msg, &xml_error_line, &xml_error_column ) )
			throw QString().append( tr( "Ошибка при анализе xml, сообщение: %1, строка: %2, столбец: %3\n" ).arg( xml_error_msg ).arg( xml_error_line ).arg( xml_error_column ) );

		const QDomElement docElement = doc.documentElement( );
		//// определяем цвет бек-граунда
		//const QDomNode dn_no_workspace = doc_el.namedItem( tag::no_workspace );
		//if ( !dn_no_workspace.isNull( ) )
		//{
		//	m_workspace.m_no_back_color = convert_to_color( dn_no_workspace, tag::back_color, m_workspace.m_no_back_color );
		//	const QDomNode dn_up = dn_no_workspace.namedItem( tag::user_properties );
		//	if ( !dn_up.isNull( ) )
		//	{
		//		m_workspace.m_no_user_property = dn_up.toElement( ).text( );
		//	}
		//}
		//load_workspace_from_xml( doc_el.namedItem( tag::workspace ) );
		loadFigures( docElement.elementsByTagName( tag::figure ), load_allways );
		//load_connectors_from_xml( doc_el.elementsByTagName( tag::line ), ret, load_allways );			
		//load_text_labels( doc_el.elementsByTagName( tag::text_label ), current_doc.documentElement().childNodes() );
	}
	catch (std::exception* e)
	{
		Q_EMIT ErrorOccur( tr("LoadScheme: %1").arg(e->what()) );
	}
	catch (const QString &errorString)
	{
		return errorString;
	}
	catch ( ... )
	{
		Q_EMIT ErrorOccur( tr("LoadScheme: Неизвестная ошибка") );
	}		

	return "";

}

void OptimaView::beforeExecute1CCall()
{

}
