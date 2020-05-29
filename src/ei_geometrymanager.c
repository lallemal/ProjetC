/******************************************************************************
* File:             ei_geometrymanager.c
*
* Author:           Robin Bertin, Aymeric Devriésère, Louise Lallemand
* Created:          05/26/20
* Description:      
*****************************************************************************/

#include "ei_placer.h"
#include "ei_utils.h"
#include "utils.h"
#include "stdlib.h"
#include "string.h"
ei_geometrymanager_t sentinel_geo = {"sentinel", NULL, NULL, NULL};
ei_geometrymanager_t *actual = NULL;

void	                ei_geometrymanager_register	(ei_geometrymanager_t* geometrymanager){
        if (!is_defined(ei_geometrymanager_from_name(geometrymanager->name))){
                sentinel_geo.next = geometrymanager;
                geometrymanager->next = actual;
                actual = geometrymanager;
        }
}

ei_geometrymanager_t*	ei_geometrymanager_from_name	(ei_geometrymanager_name_t name){
        ei_geometrymanager_t *choice = &sentinel_geo;
        while (choice != NULL){
                if(!strcmp(name, choice->name)){
                        return choice;
                }
                choice = choice->next;
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
        //when the parent is not followed anymore by a manager, his children are released too
        ei_widget_t                     *child;
        child = widget->children_head;
        while (child){
                ei_geometrymanager_unmap(child);
                child = child->next_sibling;
        }

        widget->screen_location = ei_rect_zero();
}

void 			ei_register_placer_manager 	(void){
        ei_geometrymanager_t *new_geomanager = safe_malloc(sizeof(ei_geometrymanager_t));

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
                if (widget->geom_params->manager == ei_geometrymanager_from_name("placer")){
                        to_configure = (ei_placer_t *)(widget->geom_params);

                } else {
                        ei_geometrymanager_unmap(widget);
                }


        }

        if (!is_defined(widget->geom_params)){
                to_configure = safe_malloc(sizeof(ei_placer_t));
                widget->geom_params = (ei_geometry_param_t *)to_configure;
                set_defaults_placer(widget);
        }
        to_configure->manager           = (ei_geometrymanager_t *)ei_geometrymanager_from_name("placer");
        to_configure->anchor            = is_defined(anchor)    ? *anchor       : to_configure->anchor ;
        to_configure->x                 = is_defined(x)         ? *x            : to_configure->x;
        to_configure->y                 = is_defined(y)         ? *y            : to_configure->y;
        to_configure->height            = is_defined(height)    ? *height       : to_configure->height;
        to_configure->width             = is_defined(width)     ? *width        : to_configure->width;
        to_configure->rel_x             = is_defined(rel_x)     ? *rel_x        : to_configure->rel_x;
        to_configure->rel_y             = is_defined(rel_y)     ? *rel_y        : to_configure->rel_y;
        to_configure->rel_width         = is_defined(rel_width) ? *rel_width    : to_configure->rel_width ;
        to_configure->rel_height        = is_defined(rel_height)? *rel_height   : to_configure->rel_width ;

        to_configure->manager->runfunc(widget);
}

