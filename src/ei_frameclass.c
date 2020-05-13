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
	ei_widget_t	widget;

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
        frame->color = {0xff, 0xff, 0xff, 0xff};
        frame->border_width = 0;
        frame->relief = NULL;
        frame->text_font = NULL;
        frame->text_anchor = NULL;
        frame->img = NULL;
        frame->img_rect = NULL;
        frame->img_anchor = NULL;
}


void ei_frame_drawnfunc(struct	ei_widget_t*	widget,
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
	frame.drawfunc = &ei_frame_drawnfunc;
}
