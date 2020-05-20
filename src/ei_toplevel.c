//
// Created by aymeric on 19/05/2020.
//
#include "ei_toplevel.h"
#include "utils.h"
#include "ei_utils.h"
#include "hw_interface.h"
#include "ei_application.h"
#include "ei_types.h"


void*                    ei_toplevel_allofunc                            (void){
        ei_toplevel *new_top_level = safe_malloc(sizeof(ei_toplevel));
        return new_top_level;
}

void                    ei_toplevel_releasefunc                         (ei_widget_t*	widget){
        do_nothing();
}

void                    ei_toplevel_setdefaultsfunc                     (ei_widget_t* widget){
        ei_toplevel *to_configure               = (ei_toplevel *)widget;
        to_configure->min_size                  = ei_rect_zero().size;
        to_configure->title                     = NULL;
        to_configure->is_closable               = 0;
        to_configure->resize_params             = ei_axis_none;
        to_configure->widget.screen_location    = ei_rect_zero();
        to_configure->widget.content_rect       = &(to_configure->widget.screen_location);
        to_configure->border                    = 2;
        to_configure->en_tete                   = ei_size(0, 15);
        to_configure->title_font                = ei_default_font;

        // changer le content rect pour que ca commende en dessous de l'entete


}


void                    ei_toplevel_geomnotifyfunc                      (struct ei_widget_t* widget){
        ei_app_invalidate_rect(&(widget->screen_location));
}


void                    ei_toplevel_drawfunc                            (struct	ei_widget_t*	widget,
                                                                                ei_surface_t	surface,
                                                                                ei_surface_t	pick_surface,
                                                                                ei_rect_t*	clipper){
        hw_surface_lock(surface);
        ei_toplevel *to_draw            = (ei_toplevel *)widget;
        ei_rect_t   *rect_to_fill       = safe_malloc(sizeof(ei_rect_t));
        ei_rect_t   rect_tot;


        if (!is_defined(clipper)){
                rect_tot = hw_surface_get_rect(surface);
        } else {
                rect_tot = inter_rect(clipper, &widget->screen_location);;
        }
        rect_to_fill->top_left.x        = rect_tot.top_left.x;
        rect_to_fill->top_left.y        = rect_tot.top_left.y           + to_draw->en_tete.height;
        rect_to_fill->size.width        = rect_tot.size.width           - 2*(to_draw->border);
        rect_to_fill->size.height       = rect_tot.size.height          - 2*(to_draw->border);

        if (is_defined(to_draw->title)){

                int text_width;
                int text_height;
                hw_text_compute_size(to_draw->title, to_draw->title_font, &text_width, &text_height);
        }



}