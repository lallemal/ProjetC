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
	int             font_size;
	ei_fontstyle_t  fontstyle;
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
	frame->fontstyle = ei_style_normal;
	frame->font_size = ei_font_default_size;
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
                ei_rect_t* rect_surface= malloc(sizeof(ei_rect_t));
                *rect_surface = hw_surface_get_rect(surface);
                rect_to_fill->top_left.x = rect_surface->top_left.x + frame->border_width;
                rect_to_fill->top_left.y = rect_surface->top_left.y + frame->border_width;
                rect_to_fill->size.width = rect_surface->size.width - 2*(frame->border_width);
                rect_to_fill->size.height = rect_surface->size.height - 2*(frame->border_width);
                *rect_tot = *rect_surface;
                free(rect_surface);
        }
        else{
                int x_clipper= clipper->top_left.x;
                int y_clipper= clipper->top_left.y;
                int height_clipper = clipper->size.height;
                int width_clipper = clipper->size.width;
                int x_frame= widget->screen_location.top_left.x;
                int y_frame = widget->screen_location.top_left.y;
                int height_frame = widget->screen_location.size.height;
                int width_frame = widget->screen_location.size.width;

                rect_to_fill->top_left.x=max(x_frame, x_clipper) + frame->border_width;
                rect_to_fill->top_left.y=max(y_clipper, y_frame) +frame->border_width;
                rect_to_fill->size.width=abs(min(x_clipper+width_clipper, x_frame+width_frame)-max(x_clipper, x_frame))-2*(frame->border_width);
                rect_to_fill->size.height=abs(max(y_frame, y_clipper)-min(y_frame+height_frame, y_clipper+height_clipper))-2*(frame->border_width);

                rect_tot->top_left.x=max(x_frame, x_clipper);
                rect_tot->top_left.y=max(y_clipper, x_clipper);
                rect_tot->size.width=abs(min(x_clipper+width_clipper, x_frame+width_frame)-max(x_clipper, x_frame));
                rect_tot->size.height=abs(max(y_frame, y_clipper)-min(y_frame+height_frame, y_clipper+height_clipper));
        }
        //On récupère la couleur du fond
        ei_color_t color_back = frame->color;

        //On récupère le point qui rattache le texte
        //Si clipper==NULL =>  rect_to_fill== surface - bordure
        //si text=!NULL
        ei_point_t* point_text=malloc(sizeof(ei_point_t));
        int* height_text=malloc(sizeof(int));
        int* width_text = malloc(sizeof(int));
        if (frame->text != NULL){
                hw_text_compute_size(frame->text, frame->text_font, width_text, height_text);
                int64_t height_text_int = (int64_t) height_text;
                int64_t width_text_int = (int64_t) width_text;
                point_text = anchor_point(surface, rect_to_fill, frame->text_anchor, width_text_int,height_text_int);

        }
        free(height_text);
        free(width_text);

        //On récupère le point qui rattache l'image
        //Si clipper==NULL =>  rect__to_fill== surface - bordure
        //si img=!NULL
        ei_point_t* point_img=malloc(sizeof(ei_point_t));
        if (frame->img != NULL){
                point_img = anchor_point(surface, rect_to_fill, frame->img_anchor, frame->img_rect->size.width, frame->img_rect->size.height);
        }

        //Création de relief
        int border= frame->border_width;
        //on considère le rectangle total (bordure comprise) (rect_tot)
        if (border>0) {

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
                        color_to_fill = dark_color(color_back);
                        ei_fill(surface, &color_to_fill, rect_tot);

                }

        }

        //remplissage de rect_to_fill
        ei_fill(surface, &color_back, rect_to_fill);

        //mise en place du texte
        if (frame->text != NULL){
                ei_draw_text(surface, point_text, frame->text, frame->text_font, frame->text_color, rect_to_fill);
        }
        free(point_text);

        //mise en place de l'image
        ei_rect_t* rect_img=malloc(sizeof(ei_rect_t));
        if (frame->img != NULL){
                hw_surface_lock(frame->img);
                rect_img->top_left = *point_img;
                rect_img->size.width= min(rect_to_fill->size.width, frame->img_rect->size.width);
                rect_img->size.height=min(rect_to_fill->size.height, frame->img_rect->size.height);
                ei_copy_surface(surface, rect_img, frame->img, frame->img_rect, hw_surface_has_alpha(frame->img));
                hw_surface_unlock(frame->img);
        }
        free(point_img);

        //mise en place de la liste contenant les rectangles à update
        //pour le texte et la couleur (si il y a du texte)

        if (border == 0) {
                ei_app_invalidate_rect(rect_to_fill);

                //pour l'image (si il y a une image
                if (frame->img != NULL) {
                        ei_app_invalidate_rect(rect_img);

                }
        }
        else{
                ei_app_invalidate_rect(rect_tot);
                ei_app_invalidate_rect(rect_to_fill);

                //pour l'image (si il y a une image
                if (frame->img != NULL) {
                        ei_app_invalidate_rect(rect_img);
                }
        }


        //on unlock les changements
        hw_surface_unlock(surface);

        //on libère la mémoire
        free(rect_to_fill);
        free(rect_img);
        free(rect_tot);

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
