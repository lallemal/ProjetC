//
// Created by devri on 13/05/2020.
//

#include "ei_placer.h"
#include "utils.h"


int  is_set_relative(float rel){
        return rel != -1.0;
}

void run_func(struct ei_widget_t*	widget){
        ei_placer_t     *datas          = widget->geom_params;
        ei_rect_t       *sreen_loc      = &(widget->screen_location);
        ei_rect_t       *container      = widget->parent->content_rect;

        ei_point_t      *basic_pos      = &container->top_left;

        if (widget->parent != NULL){
                if (is_set_relative(datas->rel_width)){
                        sreen_loc->size.width = datas->rel_width * container->size.width;
                } else{
                        sreen_loc->size.width = datas->width;
                }

                if (is_set_relative(datas->rel_height)){
                        sreen_loc->size.height = datas->rel_height * container->size.height;
                } else{
                        sreen_loc->size.height = datas->height;
                }

                switch (datas->anchor) {
                        case ei_anc_west:
                                sreen_loc->top_left.x = container->size.width;
                                break;
                        case ei_anc_center:
                                break;
                        case ei_anc_north:
                                break;
                        case ei_anc_south:
                                break;
                        case ei_anc_east:
                                break;
                        case ei_anc_northwest:
                                break;
                        case ei_anc_northeast:
                                break;
                        case ei_anc_southeast:
                                break;
                        case ei_anc_southwest:
                                break;


                }
        }


}

void release_func(struct ei_widget_t*	widget){

}
