/******************************************************************************
* File:             ei_widgetclass.c
*
* Author:           Robin BERTIN (Nunwan)
* Created:          05/12/20
* Description:      
*****************************************************************************/
#include <stdlib.h>
#include <string.h>


#include "ei_widgetclass.h"
#include "ei_widget.h"

typedef struct ei_frame {
	ei_widget_t		widget;
	ei_color_t		color;
	int			border_width;
	ei_relief_t		relief;
	char*			text;
	ei_font_t		text_font;
	ei_anchor_t		text_anchor;
	ei_color_t		text_color;
	ei_surface_t		img;
	ei_rect_t*		img_rect;
	ei_anchor_t		img_anchor;
} ei_frame_t;




void* ei_frame_allocfunc(void)
{
	void *newFrame = calloc(1, sizeof(ei_frame_t));
	return newFrame;
}


void ei_frame_releasefunc(ei_widget_t*	widget)
{
	ei_frame_t *frame = (ei_frame_t *) widget;
	free(&(frame->color));
	free(&(frame->border_width));
	free(&(frame->relief));
	free(&(frame->text));
	free(&(frame->text_font));
	free(&(frame->text_anchor));
	free(&(frame->img));
	free(&(frame->img_rect));
	free(&(frame->img_anchor));
}


void ei_frame_setdefaultsfunc(ei_widget_t* widget)
{
        ei_frame_t* frame= (ei_frame_t*) widget;
        frame->text = NULL;
        frame->color.red = 0;
        frame->color.green = 0;
        frame->color.blue = 0;
        frame->color.alpha = 0xff;
        frame->border_width = 0;
        frame->relief = ei_relief_none;
        frame->text_font = NULL;
        frame->text_anchor = ei_anc_none;
        frame->img = NULL;
        frame->img_rect = NULL;
        frame->img_anchor = ei_anc_none;
}


void ei_frame_drawfunc(struct	ei_widget_t*	widget,
				ei_surface_t	surface,
				ei_surface_t	pick_surface,
				ei_rect_t*	clipper)
{

}



void ei_frame_register_class(void)
{
	ei_widgetclass_t frame;
	strcpy(frame.name, "frame");
	frame.allocfunc = &ei_frame_allocfunc;
	frame.setdefaultsfunc = &ei_frame_setdefaultsfunc;
	frame.releasefunc = &ei_frame_releasefunc;
	frame.drawfunc = &ei_frame_drawfunc;

	ei_widgetclass_register(&frame);
}


void ei_frame_configure(ei_widget_t*		widget,
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
	ei_frame_t* frame = (ei_frame_t *) widget;
	if (requested_size != NULL) {
		widget->screen_location.size = *requested_size;
	}
	if (color != NULL) {
		frame->color = *color;
	}
	if (border_width != NULL) {
		frame->border_width = *border_width;
	}
	if (relief != NULL) {
		frame->relief = *relief;
	}
	if (text != NULL) {
		frame->text = *text;
	}
	if (text_font != NULL) {
		frame->text_font = *text_font;
	}
	if (text_color != NULL) {
		frame->text_color = *text_color;
	}
	if (img != NULL) {
		frame->img = *img;
	}
	if (img_rect != NULL) {
		frame->img_rect = *img_rect;
	}
	if (img_anchor != NULL) {
		frame->img_anchor = *img_anchor;
	}
}
