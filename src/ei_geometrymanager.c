//
// Created by devri on 13/05/2020.
//
#include "ei_geometrymanager.h"

ei_geometrymanager_t *actual = NULL;

void	                ei_geometrymanager_register	(ei_geometrymanager_t* geometrymanager){
        geometrymanager->next = actual;
        actual = geometrymanager;
}

ei_bool_t  is_name_equal(ei_geometrymanager_name_t actual_name, ei_geometrymanager_name_t name){
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

}

void 			ei_register_placer_manager 	(void){
        ei_geometrymanager_t *new_one = malloc(sizeof(ei_geometrymanager_t));
        ei_geometrymanager_register(new_one);
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

}