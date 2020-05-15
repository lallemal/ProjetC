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
#include "ei_draw.h"


#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

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
	//free(&(frame->color));
	//free(&(frame->border_width));
	//free(&(frame->relief));
	//free(&(frame->text_font));
	//free(&(frame->text_anchor));
	//free(&(frame->text_color));
	//free(&(frame->img));
	if (frame->img_rect != NULL) {
		free(frame->img_rect);
	}
	if (frame->text != NULL) {
		free(frame->text);
	}
	// free(&(frame->img_anchor));
}


void ei_frame_setdefaultsfunc(ei_widget_t* widget)
{
        ei_frame_t* frame= (ei_frame_t*) widget;
        frame->text = NULL;
        frame->color = ei_default_background_color;
        frame->border_width=0;
        frame->relief = ei_relief_none;
        frame->text_font = ei_default_font;
        frame->text_anchor = ei_anc_center;
        frame->text_color = ei_font_default_color;
        frame->img = NULL;
        frame->img_rect = NULL;
        frame->img_anchor = ei_anc_center;
	frame->widget.screen_location.top_left.x = 0;
	frame->widget.screen_location.top_left.y = 0;
}


/**
 * @brief	Fonction to know where is the point at the top right corner in the rectangle
 *
 * @param	rect		Global rectangle which will contain the text or the image
 * @param	anchor		The anchor link to the text or the image.
 * @param	width_text	width of the text or the image
 * @param	height_text	height of the text or the image
 *
 * @return			The point at the top right of the text or the image
 */
ei_point_t* anchor_point(ei_rect_t* rect, ei_anchor_t anchor, int width_text, int height_text)
{
        ei_point_t* point=malloc(sizeof(ei_point_t));
        int x_rect=rect->top_left.x;
        int y_rect=rect->top_left.y;
        int width=rect->size.width;
        int height=rect->size.height;
        if (anchor == ei_anc_center){
                point->x=min((x_rect+x_rect+width)/2, x_rect+width-width_text);
                point->y=min((y_rect+y_rect+height)/2, y_rect+height-height_text);
        };
        if(anchor == ei_anc_north){
                point->x=min((x_rect+x_rect+width)/2, x_rect+width-width_text);
                point->y=y_rect;
        };
        if(anchor == ei_anc_northeast){
                point->x=x_rect+width-width_text;
                point->y=y_rect;
        };
        if(anchor == ei_anc_east){
                point->x=x_rect+width-width_text;
                point->y=min((y_rect+y_rect+height)/2, y_rect+height-height_text);
        };
        if(anchor == ei_anc_southeast){
                point->x=x_rect+width-width_text;
                point->y=y_rect+height-height_text;
        };
        if(anchor == ei_anc_south){
                point->x=min((x_rect+x_rect+width)/2, x_rect+width-width_text);
                point->y=y_rect+height-height_text;
        };
        if(anchor == ei_anc_southwest){
                point->x=x_rect;
                point->y=y_rect+height-height_text;
        };
        if(anchor == ei_anc_west){
                point->x=x_rect;
                point->y=min((y_rect+y_rect+height)/2, y_rect+height-height_text);
        };
        if(anchor == ei_anc_northwest){
                point->x=x_rect;
                point->y=y_rect;
        };
        return point;
}



void ei_frame_drawfunc(struct	ei_widget_t*	widget,
				ei_surface_t	surface,
				ei_surface_t	pick_surface,
				ei_rect_t*	clipper)
{
        ei_frame_t* frame=(ei_frame_t*)widget;
        hw_surface_lock(surface);
        ei_color_t color_back = frame->color;
	ei_rect_t *rect_to_fill;
	if (clipper != NULL) {
		int x_clipper= clipper->top_left.x;
		int y_clipper= clipper->top_left.y;
		int height_clipper = clipper->size.height;
		int width_clipper = clipper->size.width;
		int x_frame= widget->screen_location.top_left.x;
		int y_frame = widget->screen_location.top_left.y;
		int height_frame = widget->screen_location.size.height;
		int width_frame = widget->screen_location.size.width;
		rect_to_fill = malloc(sizeof(ei_rect_t));

		rect_to_fill->top_left.x=max(x_frame, x_clipper);
		rect_to_fill->top_left.y=max(y_clipper, y_frame);
		rect_to_fill->size.width=abs(min(x_clipper+width_clipper, x_frame+width_frame)-max(x_clipper, x_frame));
		rect_to_fill->size.height=abs(max(y_frame, y_clipper)-min(y_frame+height_frame, y_clipper+height_clipper));
		ei_fill(surface, &color_back, rect_to_fill);
	}
	else {
		ei_fill(surface, &color_back, NULL);
	}


	if (frame->text != NULL) {
		int* height_text;
		height_text=malloc(sizeof(int));
		int* width_text;
		width_text = malloc(sizeof(int));
		hw_text_compute_size(frame->text, frame->text_font, width_text, height_text);
		int64_t height_text_int= (int64_t)height_text;
		int64_t width_text_int= (int64_t)width_text;
		// RAJOUT D'une condition si clipper NULL
		ei_point_t* point= anchor_point(rect_to_fill, frame->text_anchor, width_text_int, height_text_int);
		ei_draw_text(surface, point, frame->text, frame->text_font, frame->text_color, rect_to_fill);

		free(height_text);
		free(width_text);
	}

	if (clipper != NULL) {
		ei_linked_rect_t *liste_rect1=malloc(sizeof(ei_linked_rect_t));
		liste_rect1->rect=*rect_to_fill;
		liste_rect1->next=NULL;
		if (frame->img != NULL) {
			hw_surface_lock(frame->img);
			// Rajout d'une condition si clipper NULL
			ei_point_t* point_img=anchor_point(rect_to_fill, frame->img_anchor, frame->img_rect->size.width, frame->img_rect->size.height);
			ei_rect_t* rect_img=malloc(sizeof(ei_rect_t));
			rect_img->top_left = *point_img;
			rect_img->size.width= min(rect_to_fill->size.width, frame->img_rect->size.width);
			rect_img->size.height=min(rect_to_fill->size.height, frame->img_rect->size.height);
			ei_copy_surface(surface, rect_img, frame->img, frame->img_rect, hw_surface_has_alpha(frame->img));
			hw_surface_unlock(frame->img);
			ei_linked_rect_t *liste_rect2=malloc(sizeof(ei_linked_rect_t));
			liste_rect2->next->rect=*rect_img;
			liste_rect2->next->next=NULL;
			liste_rect1->next=liste_rect2;

			free(liste_rect2);
		}
        hw_surface_unlock(surface);
        hw_surface_update_rects(surface, liste_rect1);
	free(rect_to_fill);
	free(liste_rect1);
	}
	else {
		hw_surface_unlock(surface);
		hw_surface_update_rects(surface, NULL);
	}




}



void ei_frame_register_class(void)
{
	ei_widgetclass_t* frame = malloc(sizeof(ei_widgetclass_t));
	strcpy(frame->name, "frame");
	frame->allocfunc = &ei_frame_allocfunc;
	frame->releasefunc = &ei_frame_releasefunc;
	frame->drawfunc = &ei_frame_drawfunc;
	frame->setdefaultsfunc = &ei_frame_setdefaultsfunc;
	frame->next = NULL;
	ei_widgetclass_register(frame);
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
