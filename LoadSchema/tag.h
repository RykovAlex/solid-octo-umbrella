#pragma once

namespace tag
{
	namespace data 
	{
		enum {
			uuid = 0
			,uuidOwner
		};
	}
	
	static const QString back_color( "BackColor" );
	static const QString gradient_color( "GradientColor" );
	static const QString gradient_type( "GradientType" );
	static const QString back_line( "BackLine" );
	static const QString bold_font( "BoldFont" );
	static const QString real_step ( "RealStep" );
	static const QString virtual_step ( "VirtualStep" );
	static const QString id( "ID" );
	static const QString id_owner( "IDOwner" );
	static const QString link_to_owner( "LinkToOwner" );
	static const QString italic_font( "ItalicFont" );
	static const QString kx( "KX" );
	static const QString ky( "KY" );
	static const QString name_font( "NameFont" );
	static const QString paint_line( "PaintLine" );
	static const QString picture( "Picture" );
	static const QString coordinate( "Coordinate" );
	static const QString picture_coordinate( "PictureCoordinate" );
	static const QString pos_coordinate( "PosCoordinate" );
	static const QString size_font( "SizeFont" );
	static const QString strikethru_font( "StrikethruFont" );
	static const QString structure_dot( "StructureDot" );
	static const QString text( "Text" );
	static const QString halign( "HAlign" );
	static const QString text_color( "TextColor" );
	static const QString text_back_color( "BackColor" );
	static const QString text_label( "TextLabel" );
	static const QString figure( "Figure" );
	static const QString line( "Line" );	
	static const QString thickness_line( "ThicknessLine" );
	static const QString type_line( "TypeLine" );
	static const QString underline_font( "UnderlineFont" );
	static const QString workspace( "Workspace" );
	static const QString no_workspace( "Noworkspace" );
	static const QString user_properties( "UserProperties" );
	static const QString no( "NO" );
	static const QString angle( "Angle" );
	static const QString text_width( "TextWidth" );
	static const QString scale_pic_to_owner( "ScalePicToOwner" );
	static const QString cross_line_enable( "CrossLineEnable" );
	static const QString transparent( "Transparent" );
	static const QString picture_height( "PictureHeight" );
	static const QString picture_width ( "PictureWidth" );
	static const QString progname ( "ProgName" );
	static const QString progversion ( "ProgVersion" );
	static const QString scale ( "Scale" );
	static const QString center ( "Center" );
	static const QString lhdot ( "Lhdot" );
	static const QString drawing_connector ( "DrawingConnector" );
	static const QString view_mode( "ViewMode" );
	static const QString picture_linked( "PictureLinked" );
	static const QString bidirect_resize( "BiDirectResize" );
	static const QString double_line( "DoubleLine" );
	static const QString drop_shadow( "DropShadow" );
	static const QString custom_width( "CustomWidth" );
	static const QString owner_width( "OwnerWidth" );
	static const QString percent_width_to_owner( "PercentWidthToOwner" );
	static const QString type_setting_width( "TypeSettingWidth" );
	static const QString order( "Order" );
	static const QString size_shape_begin("SizeShapeBegin");
	static const QString size_shape_end("SizeShapeEnd");
	static const QString shape_begin("ShapeBegin");
	static const QString shape_end("ShapeEnd");
	static const QString cross_type("CrossType");
	static const QString radius_corner("RadiusCorner");
	static const QString type("Type");

	inline QString to_string( const QColor& cl )
	{
		const QString cl_name = cl.name( );
		return QString( "%1%2" ).arg( cl_name.right( 6 ) ).arg( cl.alpha( ), 2, 16, QLatin1Char( '0' ) );
	}
}
