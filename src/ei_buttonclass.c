/******************************************************************************
* File:             ei_widgetclass.c
*
* Author:           Robin Bertin, Aymeric Devriésère, Louise Lallemand
* Created:          05/12/20
* Description:      All functions and structure for button type widget
*****************************************************************************/
#include <stdlib.h>
#include <string.h>


#include "ei_widgetclass.h"
#include "ei_widget.h"
#include "draw_tools.h"
#include "ei_draw.h"
#include "utils.h"
#include "math.h"
#include "ei_application.h"
#include "ei_event.h"
#include "ei_toplevel.h"
#include "callfunction.h"


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

	ei_callback_t	callback;
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
	if (button->img_rect != NULL) {
		free(button->img_rect);
	}
	if (button->text != NULL) {
		free(button->text);
	}
	if (button->img != NULL) {
		hw_surface_free(button->img);
	}
	if (widget->pick_color) {
		free(widget->pick_color);
	}
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
        //set up of the pick color for the button
	if (widget->pick_color == NULL) {
		ei_color_t* pick_color = malloc(sizeof(ei_color_t));
		*pick_color = ei_map_color(pick_surface, widget->pick_id);
		widget->pick_color = pick_color;
	}

        ei_button_t* button=(ei_button_t*)widget;
        hw_surface_lock(surface);
        hw_surface_lock(pick_surface);
        ei_rect_t* rect_tot = malloc(sizeof(ei_rect_t));

        //rect_tot is the rectangle representating the entire button
        *rect_tot = widget->screen_location;
        //drawing of the button in the surface (relief and border_width included)
        draw_button(surface, rect_tot,  button->color, button->border_width, button->corner_radius, button->relief, clipper);
        //drawing of the button in the pick surface (only one color for the entire button)
        draw_button(pick_surface, rect_tot, *(widget->pick_color), 0, button->corner_radius, button->relief, clipper);

        //set up of the image or the text of the button
        if (button->img != NULL || button->text != NULL){
                //rect_int = the interior of the button where the text or the image will be draw
                //diférent decalages is for the move of the text or the image when the button passed from raised to sunken
                if (button->relief == ei_relief_raised) {
                        //rect_int from the top left with a width and height reduce
                        ei_rect_t* rect_int = draw_button_relief_up_down(rect_tot, button->corner_radius, button->border_width, 0, 0, round(0.1*button->border_width), round(0.1*button->border_width));

                        //set up of the text
                        if (button->text != NULL) {
                                draw_text(button->text, button->text_font, rect_int, button->text_anchor, surface,
                                          button->text_color, clipper);

                        }
                        //set up of the image
                        if (button->img != NULL) {
                                draw_image(button->img, rect_int, button->img_anchor, button->img_rect, clipper,
                                           surface);
                        }
                        free(rect_int);
                }
                if (button->relief == ei_relief_sunken){
                        //rect_int from the top left  with a width and height reduce
                        ei_rect_t* rect_int = draw_button_relief_up_down(rect_tot, button->corner_radius, button->border_width, round(0.1*button->border_width),round(0.1*button->border_width),round(0.1*button->border_width),round(0.1*button->border_width));

                        //set up of the text
                        if (button->text != NULL) {
                                draw_text(button->text, button->text_font, rect_int, button->text_anchor, surface,
                                          button->text_color, clipper);
                        }
                        //set up of the image
                        if (button->img != NULL) {
                                draw_image(button->img, rect_int, button->img_anchor, button->img_rect, clipper,
                                           surface);
                        }
                        free(rect_int);
                }
                if (button->relief ==  ei_relief_none){
                        // no decalage due to the absnece of relief
                        ei_rect_t* rect_int = draw_button_relief_up_down(rect_tot, button->corner_radius, button->border_width, 0, 0, 0, 0);

                        //set up of the text
                        if (button->text != NULL) {
                                draw_text(button->text, button->text_font, rect_int, button->text_anchor, surface,
                                          button->text_color, clipper);
                        }
                        //set u of the image
                        if (button->img != NULL) {
                                draw_image(button->img, rect_int, button->img_anchor, button->img_rect, clipper,
                                           surface);
                        }
                        free(rect_int);

                }
        }

        hw_surface_unlock(surface);
        hw_surface_unlock(pick_surface);
        free(rect_tot);
}


void ei_button_geomnotifyfunc(struct ei_widget_t* widget)
{
	ei_app_invalidate_rect(&(widget->screen_location));
}



void ei_button_register_class(void)
{
	ei_widgetclass_t* button = malloc(sizeof(ei_widgetclass_t));
	strcpy(button->name, "button");
	button->allocfunc = &ei_button_allocfunc;
	button->releasefunc = &ei_button_releasefunc;
	button->drawfunc = &ei_button_drawfunc;
	button->setdefaultsfunc = &ei_button_setdefaultsfunc;
	button->geomnotifyfunc = &ei_button_geomnotifyfunc;
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
	// Booleen to call ei_place if requested_size is changed.
	int requested_size_updated = 0;
	int ancient_border_width = button->border_width;
	// Simple changement
	if (color != NULL) {
		button->color = *color;
	}
	if (corner_radius != NULL) {
		button->corner_radius = *corner_radius;
	}
	if (relief != NULL) {
		button->relief = *relief;
	}
	if (text_font != NULL) {
		button->text_font = *text_font;
	}
	if (text_color != NULL) {
		button->text_color = *text_color;
	}
	if (text_anchor != NULL) {
		button->text_anchor = *text_anchor;
	}
	if (img_rect != NULL) {
		if (button->img_rect) {
			free(button->img_rect);
		}
		// Hard copy to enable a free of img_rect
		button->img_rect =copy_rect(*img_rect);
	}
	if (img_anchor != NULL) {
		button->img_anchor = *img_anchor;
	}
	if (callback != NULL) {
		button->callback = *callback;
	}
	if (user_param != NULL) {
		button->user_param = *user_param;
	}
	if (img != NULL) {
		if (button->img != NULL) {
			hw_surface_free(button->img);
			button->img = NULL;
		}
		// Hard copy to enable a hw_surface_free of *img
		if (*img != NULL) {
			button->img = hw_surface_create(ei_app_root_surface(), hw_surface_get_size(*img), EI_FALSE);
			ei_copy_surface(button->img, NULL, *img, NULL, EI_FALSE);
		}

		// Calcul of the requested size for the new img
		int height = 0;
		int width = 0;
		if (button->img_rect != NULL) {
			height = button->img_rect->size.height;
			width = button->img_rect->size.width;
		}
		else {
			if (button->img != NULL) {
				ei_size_t img_size = hw_surface_get_size(button->img);
				height = img_size.height;
				width = img_size.width;
			}
		}
		height += 2* ancient_border_width;
		width += 2 * ancient_border_width;
		widget->requested_size.height = max(height, widget->requested_size.height);
		widget->requested_size.width = max(width, widget->requested_size.width);
		requested_size_updated = 1;
	}
	if (text != NULL) {
		if (button->text != NULL) {
			free(button->text);
		}
		// Hard copy to enable a free of text
		if (*text != NULL) {
			button->text = malloc((strlen(*text) + 1) * sizeof(char));
			button->text = strcpy(button->text, *text);
		}
		else {
			button->text = NULL;
		}
		
		// Update the requested to fit with the text
		if (button->text != NULL) {
			int height_text;
			int width_text;
			hw_text_compute_size(button->text, button->text_font, &width_text, &height_text);
			height_text += 2 * ancient_border_width;
			width_text += 2 * ancient_border_width;
			widget->requested_size.height = max(height_text, widget->requested_size.height);
			widget->requested_size.width = max(width_text, widget->requested_size.width);
			requested_size_updated = 1;
		}

	}
	if (border_width != NULL) {
		button->border_width = *border_width;
		widget->requested_size.height -= 2 * ancient_border_width;
		widget->requested_size.height += 2 * button->border_width;
		widget->requested_size.width -= 2 * ancient_border_width;
		widget->requested_size.width += 2 * button->border_width;
		requested_size_updated = 1;
	}
	if (requested_size != NULL) {
		widget->requested_size.height = requested_size->height;
		widget->requested_size.width = requested_size->width;
		requested_size_updated = 1;
	}
	if (requested_size_updated) {
		if (widget->parent != NULL) {
			 ei_place(widget, NULL, NULL, NULL, &(widget->requested_size.width), &(widget->requested_size.height), NULL, NULL, NULL, NULL);
		}
	}
	// The button has changed so it has to be redrawn
	ei_app_invalidate_rect(&(widget->screen_location));
}


ei_bool_t button_on_release(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (strcmp(widget->wclass->name, "button") == 0) {
		ei_button_t* button = (ei_button_t *)widget;
		if (button->relief == ei_relief_sunken) {
			ei_relief_t newRelief2 = ei_relief_raised;
			ei_button_configure(widget, NULL, NULL, NULL, NULL, &newRelief2, NULL, NULL,
					NULL, NULL, NULL, NULL, NULL, NULL, NULL);
		}
		ei_unbind(ei_ev_mouse_buttonup, widget, NULL, button_on_release, NULL);
		if (button->callback != NULL) {
			ei_callback_t callback_button = button->callback;
			callback_button(widget, event, button->user_param);
		}
	}
}


ei_bool_t button_on_press(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (strcmp(widget->wclass->name, "button") == 0) {
		ei_button_t* button = (ei_button_t *) widget;
		if (button->relief == ei_relief_raised) {
			ei_relief_t newRelief = ei_relief_sunken;
			ei_button_configure(widget, NULL, NULL, NULL, NULL, &newRelief, NULL, NULL,
					NULL, NULL, NULL, NULL, NULL, NULL, NULL);
		}
		ei_bind(ei_ev_mouse_buttonup, widget, NULL, button_on_release, NULL);
	}
}
