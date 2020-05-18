//
// Created by devri on 13/05/2020.
//
#include "ei_placer.h"
#include "ei_utils.h"
#include "utils.h"
#include "stdlib.h"
#include "string.h"
ei_geometrymanager_t sentinel_geo = {"sentinel", NULL, NULL, NULL};
ei_geometrymanager_t *actual = NULL;

void	                ei_geometrymanager_register	(ei_geometrymanager_t* geometrymanager){
        sentinel_geo.next = geometrymanager;
        geometrymanager->next = actual;
        actual = geometrymanager;

}



ei_geometrymanager_t*	ei_geometrymanager_from_name	(ei_geometrymanager_name_t name){
        ei_geometrymanager_t *choice = &sentinel_geo;
        while (actual != NULL){
                if(is_name_equal(choice->name, name)){
                        return choice;
                }
                choice = actual;
        }
        return choice;
}

void			ei_geometrymanager_unmap	(ei_widget_t*		widget){
        if (is_defined(widget->geom_params)){
                widget->geom_params->manager->releasefunc(widget);
                free(widget->geom_params);
                widget->geom_params = NULL;
                widget->screen_location = ei_rect_zero();
        }
        ei_widget_t                     *child;

        while (child){
                ei_geometrymanager_unmap(child);
                child = child->next_sibling;
        }

        widget->screen_location = ei_rect_zero();
}

void 			ei_register_placer_manager 	(void){
        ei_geometrymanager_t *new_geomanager = malloc(sizeof(ei_geometrymanager_t));
        //        setting parameters
        strcpy(new_geomanager->name, "placer");
        new_geomanager->runfunc = &ei_run_func;
        new_geomanager->releasefunc = &ei_release_func;
        //      end of setting

        //      adding the new manager to the list
        ei_geometrymanager_register(new_geomanager);
}

void			ei_place			(ei_widget_t*		widget,
                                                             ei_anchor_t*		anchor,
                                                             int*			x,
                                                             int*			y,
                                                             int*			width,
                                                             int*			height,
                                                             float*			rel_x,
                                                             float*			rel_y,
                                                             float*			rel_width,
                                                             float*			rel_height){

        ei_placer_t *to_configure;

        if (is_defined(widget->geom_params)){
                if (widget->geom_params->manager->name == "placer"){
                        to_configure = (ei_placer_t *)(widget->geom_params);
                } else {
                        ei_geometrymanager_unmap(widget);
                }


        }

        if (!is_defined(widget->geom_params)){
                to_configure = safe_malloc(sizeof(ei_placer_t));

                to_configure->manager = (ei_geometry_param_t *)ei_geometrymanager_from_name("placer");
                widget->geom_params = (ei_geometry_param_t *)to_configure;
        }
        to_configure->anchor            = is_defined(anchor)    ? *anchor       : ei_anc_northwest;
        to_configure->x                 = is_defined(x)         ? *x            : 0;
        to_configure->y                 = is_defined(y)         ? *y            : 0;
        to_configure->height            = is_defined(height)    ? *height       : widget->requested_size.height;
        to_configure->width             = is_defined(width)     ? *width        : widget->requested_size.width;
        to_configure->rel_x             = is_defined(rel_x)     ? *rel_x        : 0.0;
        to_configure->rel_y             = is_defined(rel_y)     ? *rel_y        : 0.0;
        to_configure->rel_width         = is_defined(rel_width) ? *rel_width    : -1.0;
        to_configure->rel_height        = is_defined(rel_height)? *rel_height   : -1.0;

        ei_run_func(widget);
}

