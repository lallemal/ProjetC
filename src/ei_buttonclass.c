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
#include "math.h"


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


void ei_button_drawfunc(struct	ei_widget_t*	widget,
                       ei_surface_t	surface,
                       ei_surface_t	pick_surface,
                       ei_rect_t*	clipper)
{
        ei_button_t* button=(ei_button_t*)widget;
        hw_surface_lock(surface);
        ei_rect_t* rect_tot = malloc(sizeof(ei_rect_t));

        *rect_tot = widget->screen_location;

        //on trace le bouton
        draw_button(surface, pick_surface, rect_tot, *widget->pick_color,  button->color, button->border_width, button->corner_radius, button->relief);

        //On créé le rectangle 'intérieur' qui contiendra l'image ou le texte
        if (button->img != NULL || button->text != NULL){
                ei_rect_t* rect_int = malloc(sizeof(ei_rect_t));
                int center_x = rect_tot->top_left.x + button->corner_radius;
                int center_y = rect_tot->top_left.y + button->corner_radius;
                rect_int->top_left.x = center_x + button->corner_radius*cos(2*M_PI/3);
                rect_int->top_left.y = center_y + button->corner_radius*cos(2*M_PI/3);
                rect_int->size.width = rect_tot->size.width - button->corner_radius;
                rect_int->size.height = rect_tot->size.height - button->corner_radius;

                ei_rect_t* rect_int_on_screen = malloc(sizeof(ei_rect_t));
                *rect_int_on_screen = inter_rect(clipper, rect_int);

                if (button->text != NULL){
                        ei_point_t* point_text;
                        int height_text;
                        int width_text;
                        hw_text_compute_size(button->text, button->text_font, &width_text, &height_text);
                        point_text = anchor_point(rect_int, button->text_anchor, width_text,height_text);
                        ei_draw_text(surface, point_text, button->text, button->text_font, button->text_color, rect_int);
                        free(point_text);
                }
                if (button->img != NULL){
                        ei_point_t* point_img;
                        point_img = anchor_point(rect_int, button->img_anchor, button->img_rect->size.width, button->img_rect->size.height);
                        hw_surface_lock(button->img);
                        ei_rect_t *source_rectangle = button->img_rect;
                        if (point_img->x < 0) {
                                source_rectangle->top_left.x = button->img_rect->top_left.x + abs(point_img->x);
                                source_rectangle->size.width = rect_int_on_screen->size.width;
                        }
                        if (point_img->y < 0) {
                                source_rectangle->top_left.y = button->img_rect->top_left.y + abs(point_img->y);
                                source_rectangle->size.height = rect_int_on_screen->size.height;
                        }



                        if (point_img->x >= 0 && point_img->y >= 0) {
                                source_rectangle = NULL;
                        }

                        ei_rect_t* rect_img = malloc(sizeof(ei_rect_t));

                        if (button->img_rect->size.height < rect_int_on_screen->size.height && button->img_rect->size.width < rect_int_on_screen->size.width){
                                rect_img->top_left.x = point_img->x;
                                rect_img->top_left.y = point_img->y;
                                rect_img->size.width = button->img_rect->size.width;
                                rect_img->size.height = button->img_rect->size.height;
                        }
                        else {
                                rect_img->top_left.x = point_img->x;
                                rect_img->top_left.y = point_img->y;
                                rect_img->size.width = rect_int_on_screen->size.width;
                                rect_img->size.height = rect_int_on_screen->size.height;
                        }
                        ei_copy_surface(surface, rect_img, button->img, source_rectangle, hw_surface_has_alpha(button->img));

                        hw_surface_unlock(button->img);
                        free(point_img);
                        free(rect_int_on_screen);
                        free(source_rectangle);
                        free(rect_img);

                }
        }

        hw_surface_unlock(surface);
        free(rect_tot);
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

