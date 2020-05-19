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
#include "draw_tools.h"
#include "ei_draw.h"
#include "utils.h"


#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

typedef struct ei_button {
	ei_widget_t	widget;

	ei_color_t	color;
	int		border_width;
	int		corner_radius;
	ei_relief_t	relief;
	char*		text;
	ei_font_t	text_font;
	ei_anchor_t	text_anchor;
	ei_color_t      text_color;
	ei_surface_t	img;
	ei_rect_t*	img_rect;
	ei_anchor_t	img_anchor;

	ei_callback_t*	callback;
	void*		user_param;
} ei_button_t;



void* ei_button_allocfunc(void)
{
	void *newButton= calloc(1, sizeof(ei_button_t));
	return newButton;
}


void ei_button_releasefunc(ei_widget_t*	widget)
{
	ei_button_t *button = (ei_button_t *) widget;
	//free(&(button->color));
	//free(&(button->border_width));
	//free(&(button->relief));
	//free(&(button->text_font));
	//free(&(button->text_anchor));
	//free(&(button->text_color));
	//free(&(button->img));
	if (button->img_rect != NULL) {
		free(button->img_rect);
	}
	if (button->text != NULL) {
		free(button->text);
	}
	if (button->user_param != NULL) {
		free(button->user_param);
	}
	// free(&(button->img_anchor));
}


void ei_button_setdefaultsfunc(ei_widget_t* widget)
{
        ei_button_t* button= (ei_button_t*) widget;
        button->text = NULL;
        button->color = ei_default_background_color;
        button->border_width = k_default_button_border_width;
        button->relief = ei_relief_raised;
        button->text_font = ei_default_font;
        button->text_anchor = ei_anc_center;
        button->text_color = ei_font_default_color;
        button->img = NULL;
        button->img_rect = NULL;
        button->img_anchor = ei_anc_center;
	button->corner_radius = k_default_button_corner_radius;
	button->callback = NULL;
	button->user_param = NULL;
	button->widget.screen_location.top_left.x = 0;
	button->widget.screen_location.top_left.y = 0;
}


// TODO
void ei_button_drawfunc(struct	ei_widget_t*	widget,
                       ei_surface_t	surface,
                       ei_surface_t	pick_surface,
                       ei_rect_t*	clipper)
{
        ei_button_t* button=(ei_button_t*)widget;
        hw_surface_lock(surface);
        ei_rect_t *rect_to_fill=malloc(sizeof(ei_rect_t));
        ei_rect_t* rect_tot = malloc(sizeof(ei_rect_t));

        if (clipper == NULL){
                ei_rect_t* rect_surface= malloc(sizeof(ei_rect_t));
                *rect_surface = hw_surface_get_rect(surface);
                rect_to_fill->top_left.x = rect_surface->top_left.x + button->border_width;
                rect_to_fill->top_left.y = rect_surface->top_left.y + button->border_width;
                rect_to_fill->size.width = rect_surface->size.width - 2*(button->border_width);
                rect_to_fill->size.height = rect_surface->size.height - 2*(button->border_width);
                *rect_tot = *rect_surface;
                free(rect_surface);
        }
        else{
                *rect_tot = inter_rect(clipper, &widget->screen_location);
                rect_to_fill->top_left.x = rect_tot->top_left.x + button->border_width;
                rect_to_fill->top_left.y = rect_tot->top_left.y + button->border_width;
                rect_to_fill->size.width = rect_tot->size.width - 2*button->border_width;
                rect_to_fill->size.height = rect_tot->size.height - 2*button->border_width;

        }

        //on trace le bouton

}



void ei_button_register_class(void)
{
	ei_widgetclass_t* button = malloc(sizeof(ei_widgetclass_t));
	strcpy(button->name, "button");
	button->allocfunc = &ei_button_allocfunc;
	button->releasefunc = &ei_button_releasefunc;
	button->drawfunc = &ei_button_drawfunc;
	button->setdefaultsfunc = &ei_button_setdefaultsfunc;
	button->next = NULL;
	ei_widgetclass_register(button);
}


void ei_button_configure	(ei_widget_t*		widget,
				ei_size_t*		requested_size,
				const ei_color_t*	color,
				int*			border_width,
				int*			corner_radius,
				ei_relief_t*		relief,
				char**			text,
				ei_font_t*		text_font,
				ei_color_t*		text_color,
				ei_anchor_t*		text_anchor,
				ei_surface_t*		img,
				ei_rect_t**		img_rect,
				ei_anchor_t*		img_anchor,
				ei_callback_t*		callback,
				void**			user_param)

{
	ei_button_t* button = (ei_button_t *) widget;
	if (requested_size != NULL) {
		widget->screen_location.size = *requested_size;
	}
	if (color != NULL) {
		button->color = *color;
	}
	if (border_width != NULL) {
		button->border_width = *border_width;
	}
	if (corner_radius != NULL) {
		button->corner_radius = *corner_radius;
	}
	if (relief != NULL) {
		button->relief = *relief;
	}
	if (text != NULL) {
		button->text = *text;
	}
	if (text_font != NULL) {
		button->text_font = *text_font;
	}
	if (text_color != NULL) {
		button->text_color = *text_color;
	}
	if (img != NULL) {
		button->img = *img;
	}
	if (img_rect != NULL) {
		button->img_rect = *img_rect;
	}
	if (img_anchor != NULL) {
		button->img_anchor = *img_anchor;
	}
	if (callback != NULL) {
		button->callback = callback;
	}
	if (user_param != NULL) {
		button->user_param = *user_param;
	}
}

