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
#include "hw_interface.h"
#include "ei_draw.h"

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

typedef struct ei_button {
        ei_widget_t     widget;
        ei_color_t      color;
        int             border_width;
        int             corner_radius;
        ei_relief_t     relief;
        char*           text;
        ei_font_t       text_font;
        ei_color_t      text_color;
        ei_anchor_t     text_anchor;
        ei_surface_t    img;
        ei_rect_t*      img_rect;
        ei_anchor_t     img_anchor;
        ei_callback_t   callback;
        void*           user_param;
} ei_button_t;




void* ei_button_allocfunc(void) {
        void *newFrame = calloc(1, sizeof(ei_button_t));
        return newFrame;
}


void ei_button_releasefunc(ei_widget_t*	widget)
{
        ei_button_t *button = (ei_button_t *) widget;
        free(&(button->color));
        free(&(button->border_width));
        free(&(button->corner_radius));
        free(&(button->relief));
        free(&(button->text));
        free(&(button->text_font));
        free(&(button->text_color));
        free(&(button->text_anchor));
        free(&(button->img));
        free(&(button->img_rect));
        free(&(button->img_anchor));
        free(&(button->callback));
        free(&(button->user_param));
}

void ei_button_setdefaultsfunc(ei_widget_t* widget)
{
        ei_button_t* button= (ei_button_t*) widget;
        button->color = ei_default_background_color;
        button->border_width=k_default_button_border_width;
        button->corner_radius=k_default_button_corner_radius;
        button->relief = ei_relief_raised;
        button->text = NULL;
        button->text_font = ei_default_font;
        button->text_color = ei_font_default_color;
        button->text_anchor = ei_anc_center;
        button->img = NULL;
        button->img_rect = NULL;
        button->img_anchor = ei_anc_center;
        button->callback=NULL;
        button->user_param=NULL;
}



void			ei_button_configure		(ei_widget_t*		widget,
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
                button->callback = *callback;
        }
        if (user_param != NULL) {
                button->user_param = *user_param;
        }
}


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



void ei_button_drawfunc(struct	ei_widget_t*	widget,
                       ei_surface_t	surface,
                       ei_surface_t	pick_surface,
                       ei_rect_t*	clipper)
{
        ei_button_t* button=(ei_button_t*)widget;
        hw_surface_lock(surface);
        hw_surface_lock(button->img);
        ei_color_t color_back = button->color;
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

        int* height_text;
        height_text=malloc(sizeof(int));
        int* width_text;
        width_text = malloc(sizeof(int));
        hw_text_compute_size(button->text, button->text_font, width_text, height_text);
        int64_t height_text_int= (int64_t)height_text;
        int64_t width_text_int= (int64_t)width_text;
        ei_point_t* point= anchor_point(rect_to_fill, button->text_anchor, width_text_int, height_text_int);
        ei_draw_text(surface, point, button->text, button->text_font, button->text_color, rect_to_fill);


        ei_point_t* point_img=anchor_point(rect_to_fill, button->img_anchor, button->img_rect->size.width, button->img_rect->size.height);
        ei_rect_t* rect_img=malloc(sizeof(ei_rect_t));
        rect_img->top_left = *point;
        rect_img->size.width= min(rect_to_fill->size.width, button->img_rect->size.width);
        rect_img->size.height=min(rect_to_fill->size.height, button->img_rect->size.height);
        ei_copy_surface(surface, rect_img, button->img, button->img_rect, hw_surface_has_alpha(button->img));


        ei_linked_rect_t *liste_rect1=malloc(sizeof(ei_linked_rect_t));
        liste_rect1->rect=*rect_to_fill;
        liste_rect1->next=NULL;
        ei_linked_rect_t *liste_rect2=malloc(sizeof(ei_linked_rect_t));
        liste_rect2->next->rect=*rect_img;
        liste_rect2->next->next=NULL;
        liste_rect1->next=liste_rect2;
        hw_surface_unlock(surface);
        hw_surface_unlock(button->img);
        hw_surface_update_rects(surface, liste_rect1);
}

void ei_button_arc(int center, int radius, int corner_beg, int corner_end)
{
        
}

        void ei_button_rounded_frame( ei_rect_t rect, int radius)
{

}

void ei_button_register_class(void)
{
        ei_widgetclass_t button;
        strcpy(button.name, "button");
        button.allocfunc = &ei_button_allocfunc;
        button.setdefaultsfunc = &ei_button_setdefaultsfunc;
        button.releasefunc = &ei_button_releasefunc;
        button.drawfunc = &ei_button_drawfunc;

        ei_widgetclass_register(&button);
}


