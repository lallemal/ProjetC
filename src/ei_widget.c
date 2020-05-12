/******************************************************************************
* File:             ei_widget.c
*
* Author:           Robin BERTIN (Nunwan)
* Created:          05/12/20
* Description:      Implementation for widgets management.
*****************************************************************************/

#include "ei_widget.h"

ei_widget_t*	ei_widget_create	(ei_widgetclass_name_t	class_name,
					 ei_widget_t*		parent,
					 void*			user_data,
					 ei_widget_destructor_t	destructor)
{

}





void		ei_frame_configure	(ei_widget_t*		widget,
					 ei_size_t*		requested_size,
					 const ei_color_t*	color,
					 int*			border_width,
					 ei_relief_t*		relief,
					 char**			text,
					 ei_font_t*		text_font,
					 ei_color_t*		text_color,
					 ei_anchor_t*		text_anchor,
					 ei_surface_t*		img,
					 ei_rect_t**		img_rect,
					 ei_anchor_t*		img_anchor)
{

}
