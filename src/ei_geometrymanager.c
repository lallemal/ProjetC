//
// Created by devri on 13/05/2020.
//
#include "ei_geometrymanager.h"
#include "ei_placer.h"

ei_geometrymanager_t *actual = NULL;

void	                ei_geometrymanager_register	(ei_geometrymanager_t* geometrymanager){
        geometrymanager->next = actual;
        actual = geometrymanager;

}

int                    is_name_equal(ei_geometrymanager_name_t actual_name, ei_geometrymanager_name_t name){
        return strcmp(actual_name, name);
}

ei_geometrymanager_t*	ei_geometrymanager_from_name	(ei_geometrymanager_name_t name){
        ei_geometrymanager_t *listof = actual;
        while (actual != NULL){
                if(is_name_equal(listof->name, name)){
                        return listof;
                }
                listof = actual;
        }
}

void			ei_geometrymanager_unmap	(ei_widget_t*		widget){
        if(widget->geom_params == NULL)
                return;
        else{
                return;
        }
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

void do_nothing(){
}

int is_defined(void *param){
        return param != NULL;
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

        ei_placer_t *customized;

        if (!widget->geom_params){
                customized = calloc(1, sizeof(ei_placer_t));
                widget->geom_params->manager = customized;
        }
        ((ei_placer_t *)widget->geom_params)->x;
        is_defined(x)           ? customized->x = *x                     : do_nothing();
        is_defined(y)           ? customized->y = *y                     : do_nothing();
        is_defined(width)       ? customized->width = *width             : do_nothing();
        is_defined(height)      ? customized->height = *height           : do_nothing();
        is_defined(rel_x)       ? customized->rel_x = *rel_x             : do_nothing();
        is_defined(rel_y)       ? customized->rel_y = *rel_y             : do_nothing();
        is_defined(rel_width)   ? customized->rel_width = *rel_width     : do_nothing();
        is_defined(rel_height)  ? customized->rel_height = *rel_height   : do_nothing();

        ei_run_func(widget);
}