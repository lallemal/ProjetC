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
#include "ei_types.h"
#include "ei_draw.h"
#include "ei_application.h"
#include "draw_tools.h"
#include "utils.h"


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
	if (frame->img != NULL) {
		hw_surface_free(frame->img);
	}
	//if (frame->img_rect != NULL) {
	//	free(frame->img_rect);
	//}
	//if (frame->text != NULL) {
	//	free(frame->text);
	//}
	// free(&(frame->img_anchor));
	if (widget->pick_color != NULL) {
		free(widget->pick_color);
	}
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
	frame->widget.content_rect = &(frame->widget.screen_location);

}



void ei_frame_geomnotifyfunc(struct ei_widget_t* widget)
{
	ei_app_invalidate_rect(&(widget->screen_location));
}


void ei_frame_drawfunc(struct	ei_widget_t*	widget,
                       ei_surface_t	surface,
                       ei_surface_t	pick_surface,
                       ei_rect_t*	clipper)
{
	if (widget->pick_color == NULL) {
		ei_color_t* pick_color = malloc(sizeof(ei_color_t));
		*pick_color = ei_map_color(pick_surface, widget->pick_id);
		widget->pick_color = pick_color;
	}

        ei_frame_t* frame = (ei_frame_t*)widget;
        hw_surface_lock(surface);
        hw_surface_lock(pick_surface);
        ei_rect_t *rect_to_fill=malloc(sizeof(ei_rect_t));
        ei_rect_t* rect_tot = malloc(sizeof(ei_rect_t));

        *rect_tot = widget->screen_location;
        //surface de picking --> GROS PORBLEME
        ei_rect_t* rect_to_pick = malloc(sizeof(ei_rect_t));
        *rect_to_pick = inter_rect(clipper, rect_tot);
        ei_fill(pick_surface, widget->pick_color, rect_to_pick);
        free(rect_to_pick);


        rect_to_fill->top_left.x = rect_tot->top_left.x + frame->border_width;
        rect_to_fill->top_left.y = rect_tot->top_left.y + frame->border_width;
        rect_to_fill->size.width = rect_tot->size.width - 2*frame->border_width;
        rect_to_fill->size.height = rect_tot->size.height - 2*frame->border_width;


        ei_rect_t* rect_to_fill_on_screen = malloc(sizeof(ei_rect_t));
        *rect_to_fill_on_screen= inter_rect(clipper, rect_to_fill);

        //Création de relief et bordure
        if (frame->border_width>0) {
                ei_rect_t* rect_tot_on_screen = malloc(sizeof(ei_rect_t));
                *rect_tot_on_screen = inter_rect(clipper, rect_tot);

                if (frame->relief == ei_relief_raised) {
                        draw_down_relief(rect_tot, surface, frame->color, EI_TRUE, rect_tot_on_screen);

                        draw_up_relief(rect_tot, surface, frame->color, EI_TRUE, rect_tot_on_screen);


                }
                if (frame->relief == ei_relief_sunken) {
                        draw_up_relief(rect_tot, surface, frame->color, EI_FALSE, rect_tot_on_screen);
                        draw_down_relief(rect_tot, surface, frame->color, EI_FALSE, rect_tot_on_screen);

                }
                if (frame->relief == ei_relief_none){
                        ei_color_t color_to_fill;
                        color_to_fill = dark_color(frame->color);
                        ei_rect_t* rect_tot_on_screen = malloc(sizeof(ei_rect_t));
                        *rect_tot_on_screen = inter_rect(clipper, rect_tot);
                        ei_fill(surface, &color_to_fill, rect_tot_on_screen);

                }
                free(rect_tot_on_screen);

        }

        //remplissage de la frame
        ei_fill(surface, &frame->color, rect_to_fill_on_screen);

        //mise en place du texte
        if (frame->text != NULL){
                draw_text(frame->text, frame->text_font, rect_to_fill, frame->text_anchor, surface, frame->text_color, clipper);
        }

        //mise en place de l'image
        if (frame->img != NULL) {
                draw_image(frame->img, rect_to_fill, frame->img_anchor, frame->img_rect, clipper, surface);
        }


        hw_surface_unlock(surface);
        hw_surface_unlock(pick_surface);

        free(rect_to_fill);
        free(rect_tot);
        free(rect_to_fill_on_screen);
}



void ei_frame_register_class(void)
{
	ei_widgetclass_t* frame = malloc(sizeof(ei_widgetclass_t));
	strcpy(frame->name, "frame");
	frame->allocfunc = &ei_frame_allocfunc;
	frame->releasefunc = &ei_frame_releasefunc;
	frame->drawfunc = &ei_frame_drawfunc;
	frame->setdefaultsfunc = &ei_frame_setdefaultsfunc;
	frame->geomnotifyfunc = &ei_frame_geomnotifyfunc;
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
	int ancient_border_width = frame->border_width;
	if (color != NULL) {
		frame->color = *color;
	}
	if (relief != NULL) {
		frame->relief = *relief;
	}
	if (text_font != NULL) {
		frame->text_font = *text_font;
	}
	if (text_anchor != NULL) {
		frame->text_anchor = *text_anchor;
	}
	if (text_color != NULL) {
		frame->text_color = *text_color;
	}
	if (img_rect != NULL) {
		frame->img_rect = *img_rect;
	}
	if (img_anchor != NULL) {
		frame->img_anchor = *img_anchor;
	}
	if (img != NULL) {
		frame->img = *img;
		int height = 0;
		int width = 0;
		if (frame->img_rect != NULL) {
			height = frame->img_rect->size.height;
			width = frame->img_rect->size.width;
		}
		else {
			if (frame->img != NULL) {
				ei_size_t img_size = hw_surface_get_size(frame->img);
				height = img_size.height;
				width = img_size.width;
			}
		}
		height += 2* ancient_border_width;
		width += 2 * ancient_border_width;
		widget->requested_size.height = max(height, widget->requested_size.height);
		widget->requested_size.width = max(width, widget->requested_size.width);
	}
	if (text != NULL) {
		frame->text = *text;
		int height_text;
		int width_text;
		hw_text_compute_size(frame->text, frame->text_font, &width_text, &height_text);
		height_text += 2 * ancient_border_width;
		width_text += 2 * ancient_border_width;
		widget->requested_size.height = max(height_text, widget->requested_size.height);
		widget->requested_size.width = max(width_text, widget->requested_size.width);

	}
	if (border_width != NULL) {
		frame->border_width = *border_width;
		widget->requested_size.height -= 2 * ancient_border_width;
		widget->requested_size.height += 2 * frame->border_width;
		widget->requested_size.width -= 2 * ancient_border_width;
		widget->requested_size.width += 2 * frame->border_width;
	}
	if (requested_size != NULL) {
		widget->requested_size.height = requested_size->height;
		widget->requested_size.width = requested_size->width;
	}
	ei_app_invalidate_rect(&(widget->screen_location));
}
