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
	ei_color_t      text_color;
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
        frame->color = ei_default_background_color;
        frame->relief = ei_relief_none;
        frame->text_font = ei_default_font;
        frame->text_anchor = ei_anc_center;
        frame->text_color = ei_font_default_color;
        frame->img = NULL;
        frame->img_rect = NULL;
        frame->img_anchor = ei_anc_center;
}


ei_point_t* anchor_point(ei_rect_t* rect, ei_anchor_t anchor)
{
        int x_rect=rect->top_left.x;
        int y_rect=rect->top_left.y;
        int width=rect->size.width;
        int height=rect->size.height;
        int x_point;
        int y_point;
        if (anchor == ei_anc_center){
                x_point=(x_rect+x_rect+width)/2;
                y_point=(y_rect+y_rect+height)/2;
        };
        if(anchor == ei_anc_north){
                x_point=(x_rect+x_rect+width)/2;
                y_point=y_rect;
        };
        if(anchor == ei_anc_northeast){
                x_point=x_rect+width;
                y_point=y_rect;
        };
        if(anchor == ei_anc_east){
                x_point=x_rect+width;
                y_point=(y_rect+y_rect+height)/2;
        };
        if(anchor == ei_anc_southeast){
                x_point=x_rect+width;
                y_point=y_rect+height;
        };
        if(anchor == ei_anc_south){
                x_point=(x_rect+x_rect+width)/2;
                y_point=y_rect;
        };
        if(anchor == ei_anc_southwest){
                x_point=x_rect;
                y_point=y_rect+height;
        };
        if(anchor == ei_anc_west){
                x_point=x_rect;
                y_point=(y_rect+y_rect+height)/2;
        };
        if(anchor == ei_anc_northwest){
                x_point=x_rect;
                y_point=y_rect;
        };
        ei_point_t* point;
        point->x=x_point;
        point->y=y_point;
        return point;
}

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

void ei_frame_drawfunc(struct	ei_widget_t*	widget,
				ei_surface_t	surface,
				ei_surface_t	pick_surface,
				ei_rect_t*	clipper)
{
        ei_frame_t* frame=(ei_frame_t*)widget;
        hw_surface_lock(surface);
        ei_color_t color_back = frame->color;
        int x_clipper= clipper->top_left.x;
        int y_clipper= clipper->top_left.y;
        int x_frame= widget->screen_location.top_left.x;
        int y_frame = widget->screen_location.top_left.y;
        int height_clipper = clipper->size.height;
        int height_frame = widget->screen_location.size.height;
        int width_clipper = clipper->size.width;
        int width_frame = widget->screen_location.size.width;

        ei_rect_t* rect_to_fill;
        rect_to_fill->top_left.x=max(x_frame, x_clipper);
        rect_to_fill->top_left.y=max(y_clipper, y_frame);
        rect_to_fill->size.width=abs(min(x_clipper+width_clipper, x_frame+width_frame)-max(x_clipper, x_frame));
        rect_to_fill->size.height=abs(max(y_frame, y_clipper)-min(y_frame+height_frame, y_clipper+height_clipper));
        ei_fill(surface, &color_back, rect_to_fill);

        ei_point_t* point= anchor_point(rect_to_fill, frame->text_anchor);
        ei_draw_text(surface, point, frame->text, frame->text_font, frame->color, rect_to_fill);
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
