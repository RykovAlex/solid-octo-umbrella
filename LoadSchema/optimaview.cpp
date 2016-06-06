#include "stdafx.h"
#include "tag.h"
#include "optimaview.h"
#include "optimaelement.h"
#include "optimafigure.h"
#include "optimaconnector.h"
#include "optimatext.h"

OptimaView::OptimaView(QWidget *parent) : QGraphicsView(parent) 
{ 
	setScene( new QGraphicsScene(parent) );
};

template <class T>
void OptimaView::load(const QDomNodeList &elements, bool loadAllways)
{
	for ( int nn = 0; nn < elements.size( ); ++nn )
	{
		const QDomNode &element = elements.at( nn );
		const QString uuid = element.namedItem( tag::id ).toElement().text();

		if (uuid.isEmpty())
		{			
			if ( !loadAllways )
				throw QString().append( tr( "� �������� �� ������ UUID" ) );
			continue;
		}

		//������ �� ����� ������� � itemUuid �� xml, ���� ��� ���, �� �������� ���
		T *item = getItem<T>(uuid);

		Q_ASSERT( nullptr != item );

		//�������� ���������� ��� ������� ������� xml �������� � �������� �������������� xml 
		//� ������������ ��������, ����� ����� �� ���������� �� �����
		item->applyXml(element);
		item->draw();
	}

}

QGraphicsItem *OptimaView::findItem(const QString &itemUuid)
{
	const QList<QGraphicsItem*> itemList = items();
	for (QList<QGraphicsItem*>::const_iterator i = itemList.constBegin(); i != itemList.constEnd(); ++i )
	{
		QGraphicsItem* item = *i;
		QString itemUuid = getUuid(item);
		if (itemUuid == itemUuid)
		{
			return item;
		}
	}

	return nullptr;
}

template <class T>
T *OptimaView::getItem(const QString &itemUuid)
{
	const QList<QGraphicsItem*> itemList = items();
	for (QList<QGraphicsItem*>::const_iterator i = itemList.constBegin(); i != itemList.constEnd(); ++i )
	{
		QGraphicsItem* item = *i;
		QString uuid = getUuid(item);
		if (itemUuid == uuid)
		{
			return dynamic_cast<T*>(item);
		}
	}
	
	T *item = new T(itemUuid);
	scene()->addItem(item);			
	
	return item; 
}

QString OptimaView::getUuid(QGraphicsItem* item)
{
	return item->data(tag::data::uuid).toString();
}


QString OptimaView::LoadScheme(const QString &filename, bool load_allways)
{
	//setScene( new QGraphicsScene );
	
	beforeExecute1CCall();
	
	try
	{
		int xml_error_column;
		int xml_error_line;
		QString xml_error_msg;

		if ( !doc.setContent( filename, &xml_error_msg, &xml_error_line, &xml_error_column ) )
			throw QString().append( tr( "������ ��� ������� xml, ���������: %1, ������: %2, �������: %3\n" ).arg( xml_error_msg ).arg( xml_error_line ).arg( xml_error_column ) );

		const QDomElement docElement = doc.documentElement( );
		//// ���������� ���� ���-�������
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
		load<OptimaFigure>( docElement.elementsByTagName( tag::figure ), load_allways );
		load<OptimaConnector>( docElement.elementsByTagName( tag::line ), load_allways );
		load<OptimaText>( docElement.elementsByTagName( tag::text_label ), load_allways );
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
		Q_EMIT ErrorOccur( tr("LoadScheme: ����������� ������") );
	}		

	return "";

}

void OptimaView::beforeExecute1CCall()
{

}
