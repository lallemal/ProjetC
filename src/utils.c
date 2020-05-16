//
// Created by devri on 15/05/2020.
//
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "utils.h"
int                     is_name_equal(ei_geometrymanager_name_t actual_name, ei_geometrymanager_name_t name){
        return strcmp(actual_name, name);
}

void                    do_nothing(){
}

int                     is_defined(void *param){
        return param != NULL;
}

void                    *safe_malloc(size_t size){
        void *ptr = calloc(1, size);

        if (!ptr && (size > 0)){
                perror("[Calloc failed]");
                exit(EXIT_FAILURE);
        }
        return ptr;
}