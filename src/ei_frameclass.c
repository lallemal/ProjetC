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
        ei_frame_t* frame=(ei_frame_t*)widget;
        hw_surface_lock(surface);
        ei_rect_t *rect_to_fill=malloc(sizeof(ei_rect_t));
        ei_rect_t* rect_tot = malloc(sizeof(ei_rect_t));
        //on prend les deux cas clipper nul et clipper non nul pour y associer un rectangle qu est concerné par les fonctions de dessins
        //dans le cas ou le clipper est NULL, on prend toute la surface comme délimitation (et on prend en compte la présence ou non d'une bordure)


        if (clipper == NULL){
                rect_to_fill->top_left.x = widget->screen_location.top_left.x + frame->border_width;
                rect_to_fill->top_left.y = widget->screen_location.top_left.y + frame->border_width;
                rect_to_fill->size.width = widget->screen_location.size.width - 2*(frame->border_width);
                rect_to_fill->size.height = widget->screen_location.size.height - 2*(frame->border_width);
                *rect_tot = widget->screen_location;

        }
        else{
                *rect_tot = widget->screen_location;
                rect_to_fill->top_left.x = rect_tot->top_left.x + frame->border_width;
                rect_to_fill->top_left.y = rect_tot->top_left.y + frame->border_width;
                rect_to_fill->size.width = rect_tot->size.width - 2*frame->border_width;
                rect_to_fill->size.height = rect_tot->size.height - 2*frame->border_width;

        }

        //On récupère le point qui rattache le texte
        //Si clipper==NULL =>  rect_to_fill== surface - bordure
        //si text=!NULL
        ei_point_t* point_text;
        int height_text;
        int width_text;
        if (frame->text != NULL){
                hw_text_compute_size(frame->text, frame->text_font, &width_text, &height_text);
                point_text = anchor_point(rect_to_fill, frame->text_anchor, width_text,height_text);

        }

        ei_rect_t* rect_to_fill_on_screen = malloc(sizeof(ei_rect_t));
        *rect_to_fill_on_screen= inter_rect(clipper, rect_to_fill);
        //On récupère le point qui rattache l'image
        //Si clipper==NULL =>  rect__to_fill== surface - bordure
        //si img=!NULL
        ei_point_t* point_img;
        if (frame->img != NULL){
                point_img = anchor_point(rect_to_fill, frame->img_anchor, frame->img_rect->size.width, frame->img_rect->size.height);
        }

        //Création de relief
        //on considère le rectangle total (bordure comprise) (rect_tot)
        if (frame->border_width>0) {

                if (frame->relief == ei_relief_raised) {
                        draw_down_relief(rect_tot, surface, frame->color, EI_TRUE);

                        draw_up_relief(rect_tot, surface, frame->color, EI_TRUE);


                }
                if (frame->relief == ei_relief_sunken) {
                        draw_up_relief(rect_tot, surface, frame->color, EI_FALSE);
                        draw_down_relief(rect_tot, surface, frame->color, EI_FALSE);

                }
                if (frame->relief == ei_relief_none){
                        ei_color_t color_to_fill;
                        color_to_fill = dark_color(frame->color);
                        ei_fill(surface, &color_to_fill, rect_tot);

                }

        }

        //remplissage de rect_to_fill
        ei_fill(surface, &frame->color, rect_to_fill_on_screen);

        //mise en place du texte
        if (frame->text != NULL){
                ei_draw_text(surface, point_text, frame->text, frame->text_font, frame->text_color, rect_to_fill_on_screen);
                free(point_text);
        }

        //mise en place de l'image
        ei_rect_t* rect_img=malloc(sizeof(ei_rect_t));
        if (frame->img != NULL) {
                hw_surface_lock(frame->img);
                ei_rect_t *source_rectangle = frame->img_rect;
                if (point_img->x < 0) {
                        source_rectangle->top_left.x = frame->img_rect->top_left.x + abs(point_img->x);
                        source_rectangle->size.width = rect_to_fill_on_screen->size.width;
                }
                if (point_img->y < 0) {
                        source_rectangle->top_left.y = frame->img_rect->top_left.y + abs(point_img->y);
                        source_rectangle->size.height = rect_to_fill_on_screen->size.height;
                }



                if (point_img->x >= 0 && point_img->y >= 0) {
                        source_rectangle = NULL;
                }

                //rect_img->top_left.x = point_img->x;
                //rect_img->top_left.y = point_img->y;
                //rect_img->size.width = widget->screen_location.size.width;
                //rect_img->size.height = widget->screen_location.size.height;

                ei_copy_surface(surface, rect_to_fill_on_screen, frame->img, source_rectangle, hw_surface_has_alpha(frame->img));

                hw_surface_unlock(frame->img);
                free(point_img);
                free(source_rectangle);
        }


        //on unlock les changements
        hw_surface_unlock(surface);

        //on libère la mémoire
        free(rect_to_fill);
        free(rect_img);
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
			ei_size_t img_size = hw_surface_get_size(frame->img);
			height = img_size.height;
			width = img_size.width;
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
