/******************************************************************************
* File:             ei_placer.c
*
* Author:           Robin Bertin, Aymeric Devriésère, Louise Lallemand
* Created:          05/26/20
* Description:      
*****************************************************************************/

//
// Created by devri on 13/05/2020.
//
#include <stdlib.h>
#include <utils.h>
#include "ei_radiobutton.h"
#include "ei_application.h"
#include "ei_placer.h"
#include "ei_utils.h"
#include "ei_toplevel.h"

int  is_set_relative(float rel){
        return rel != -1.0;
}

void set_defaults_placer(ei_widget_t *to_set){
        ei_placer_t *to_configure = (ei_placer_t *)to_set->geom_params;
        to_configure->anchor            = ei_anc_northwest;
        to_configure->x                 = 0;
        to_configure->y                 = 0;
        to_configure->height            = to_set->requested_size.height;
        to_configure->width             = to_set->requested_size.width;
        to_configure->rel_x             = 0;
        to_configure->rel_y             = 0;
        to_configure->rel_width         = -1;
        to_configure->rel_height        = -1;
}

int  are_old_and_new_diff (ei_rect_t r1, ei_rect_t r2){
        return r1.size.height != r2.size.height ||
               r1.size.width != r2.size.width ||
               r1.top_left.x != r2.top_left.x ||
               r1.top_left.y != r2.top_left.y;

}

int  button_case(struct ei_widget_t*	widget){
        if (widget->parent != NULL){
                if (widget->parent->wclass == ei_widgetclass_from_name("toplevel")){
                        ei_toplevel * parent = (ei_toplevel *)widget->parent;
                        if (parent->close_button == widget){
                                return EI_TRUE;
                        }
                        if (parent->resize_tool == widget){
                                return EI_TRUE;
                        }
                }
        }
        return EI_FALSE;
}

void  special_case(struct ei_widget_t*	widget){
        //this function is only used by the creator of the library to manage the special case of the subframe of a top level
        if (widget->wclass == ei_widgetclass_from_name("toplevel")){
                ei_toplevel *to_configure = (ei_toplevel *)widget;
                widget->content_rect->top_left.x = to_configure->widget.screen_location.top_left.x + to_configure->border_width ;
                widget->content_rect->top_left.y = to_configure->widget.screen_location.top_left.y + MARGIN_TOP * 2 + to_configure->title_height + to_configure->border_width;
                widget->content_rect->size.width = widget->screen_location.size.width - 2 * to_configure->border_width;
                widget->content_rect->size.height = widget->screen_location.size.height - to_configure->title_height - 2 * MARGIN_TOP  - 2 * to_configure->border_width;

        }
        if (widget->wclass == ei_widgetclass_from_name("radiobutton")){
                ei_radio_button *to_configure = (ei_radio_button *)widget;
                *widget->content_rect = widget->screen_location;
                widget->content_rect->size.height -= BORDER_SIZE;
        }
}

void ei_run_func(struct ei_widget_t*	widget){
        ei_placer_t     *datas          = (ei_placer_t  *)widget->geom_params;
        ei_rect_t       *container;


        if (button_case(widget)){
                container      = &widget->parent->screen_location;

        } else {
                container      = widget->parent->content_rect;
        }


        ei_rect_t       new_screen_loc  = ei_rect_zero();
        if (widget->parent != NULL){

                if (is_set_relative(datas->rel_width)){
                        new_screen_loc.size.width = datas->rel_width * container->size.width;
                } else{
                        new_screen_loc.size.width = datas->width;
                }

                if (is_set_relative(datas->rel_height)){
                        new_screen_loc.size.height = datas->rel_height * container->size.height;
                } else{
                        new_screen_loc.size.height = datas->height;
                }
                new_screen_loc.top_left = container->top_left;

                new_screen_loc.top_left.x     +=       container->size.width  * datas->rel_x    +       datas->x;
                new_screen_loc.top_left.y     +=       container->size.height * datas->rel_y    +       datas->y;



                int half_height                   =       new_screen_loc.size.height/2;
                int half_width                    =       new_screen_loc.size.width/2;

                switch (datas->anchor) {
                        case ei_anc_northwest:
                                break;
                        case ei_anc_west:
                                new_screen_loc.top_left = ei_point_sub(new_screen_loc.top_left, ei_point(0, half_height));
                                break;
                        case ei_anc_center:
                                new_screen_loc.top_left = ei_point_sub(new_screen_loc.top_left, ei_point(half_width, half_height));
                                break;
                        case ei_anc_north:
                                new_screen_loc.top_left = ei_point_sub(new_screen_loc.top_left, ei_point(half_width, 0));
                                break;
                        case ei_anc_south:
                                new_screen_loc.top_left = ei_point_sub(new_screen_loc.top_left, ei_point(half_width, 2 * half_height));
                                break;
                        case ei_anc_east:
                                new_screen_loc.top_left = ei_point_sub(new_screen_loc.top_left, ei_point(2 * half_width, half_height));
                                break;
                        case ei_anc_northeast:
                                new_screen_loc.top_left = ei_point_sub(new_screen_loc.top_left, ei_point(2 * half_width, 0));
                                break;
                        case ei_anc_southeast:
                                new_screen_loc.top_left = ei_point_sub(new_screen_loc.top_left, ei_point(2 * half_width, 2 * half_height));
                                break;
                        case ei_anc_southwest:
                                new_screen_loc.top_left = ei_point_sub(new_screen_loc.top_left, ei_point(0, 2 * half_height));
                                break;


                }
                // to avoid computation of children if geometry is the same
                if (are_old_and_new_diff(new_screen_loc, widget->screen_location)){
                        ei_app_invalidate_rect(&(widget->screen_location));
                        ei_widget_t *child = widget->children_head;
                        widget->screen_location = new_screen_loc;
                        special_case(widget);
                        widget->wclass->geomnotifyfunc(widget);
                        while(child){
                                if (is_defined(child->geom_params))
                                        widget->geom_params->manager->runfunc(child);
                                child = child->next_sibling;
                        }
                }
        }


}

void ei_release_func(struct ei_widget_t*	widget){

}
