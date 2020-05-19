//
// Created by aymeric on 19/05/2020.
//
#include "ei_toplevel.h"
#include "utils.h"
#include "ei_utils.h"
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
        // changer le content rect pour que ca commende en dessous de l'entete


}

