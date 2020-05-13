/******************************************************************************
* File:             ei_widgetclass.c
*
* Author:           Robin BERTIN (Nunwan)
* Created:          05/12/20
* Description:      
*****************************************************************************/
#include "ei_widgetclass.h"

typedef struct ei_frame {
	ei_widget_t*	widget;
	ei_color_t	color;
	int		border_width;
	ei_relief_t	relief;
	char*		text;
	ei_font_t	text_font;
	ei_anchor_t	text_anchor;
	ei_surface_t	img;
	ei_rect_t*	img_rect;
	ei_anchor_t	img_anchor;
} ei_frame_t;




void* ei_frame_allocfunc (void)
{
	size_t frame_attribute_size = 0;
	frame_attribute_size += sizof(ei_widget_t *);
	frame_attribute_size += sizeof(ei_color_t);  // color
	frame_attribute_size += sizeof(int);  // border_width
	frame_attribute_size += sizeof(ei_relief_t);  // relief
	frame_attribute_size += sizeof(char *);  // text
	frame_attribute_size += sizeof(ei_font_t);  // text_font
	frame_attribute_size += sizeof(ei_anchor_t);  // text_anchor
	frame_attribute_size += sizeof(ei_surface_t);  // img
	frame_attribute_size += sizeof(ei_rect_t *);  // img_rect
	frame_attribute_size += sizeof(ei_anchor_t);  // img_anchor
	void *memoire_frame = malloc(frame_attribute_size);
	for (void *i = memoire_frame; i < memoire_frame + frame_attribute_size;i++) {
		*i = NULL;
	}
	return memoire_frame;
}
