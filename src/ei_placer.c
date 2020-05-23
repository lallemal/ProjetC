//
// Created by devri on 13/05/2020.
//
#include <stdlib.h>
#include <utils.h>
#include "ei_application.h"
#include "ei_placer.h"
#include "ei_utils.h"
#include "ei_toplevel.h"

int  is_set_relative(float rel){
        return rel != -1.0;
}
int  are_old_and_new_diff (ei_rect_t r1, ei_rect_t r2){
        return r1.size.height != r2.size.height ||
               r1.size.width != r2.size.width ||
               r1.top_left.x != r2.top_left.x ||
               r1.top_left.y != r2.top_left.y;

}
int  special_case(struct ei_widget_t*	widget){
        if (widget->parent != NULL){
                if (widget->parent->wclass == ei_widgetclass_from_name("toplevel")){
                        ei_toplevel * parent = widget->parent;
                        if (widget->parent->content_rect == &widget->screen_location){
                                return EI_TRUE;
                        }
                        if (parent->close_button == widget){
                                return EI_TRUE;
                        }
                }
        }
        return EI_FALSE;
}
void ei_run_func(struct ei_widget_t*	widget){
        ei_placer_t     *datas          = (ei_placer_t  *)widget->geom_params;
        ei_rect_t       *container;


        if (special_case(widget)){
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

                new_screen_loc.top_left.x     +=       container->size.width  * datas->rel_x  + datas->x;
                new_screen_loc.top_left.y     +=       container->size.height * datas->rel_y +  datas->y;



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
                        widget->wclass->geomnotifyfunc(widget);
                        ei_widget_t *child = widget->children_head;
                        widget->screen_location = new_screen_loc;
                        while(child){
                                if (is_defined(child->geom_params))
                                        ei_run_func(child);
                                child = child->next_sibling;
                        }
                }
        }


}

void ei_release_func(struct ei_widget_t*	widget){

}
