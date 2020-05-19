//
// Created by devri on 15/05/2020.
//
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "utils.h"

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)


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


int                     max(int a, int b){
        if (a > b)
                return a;
        else
                return b;
}


ei_rect_t inter_rect(const ei_rect_t* rect1, const ei_rect_t* rect2)
{
	if (rect1 == NULL) {
		return *rect2;
	}
	if (rect2 == NULL) {
		return *rect1;
	}
	int x1_min = rect1->top_left.x;
	int x1_max = x1_min + rect1->size.width;
	int x2_min = rect2->top_left.x;
	int x2_max = x2_min + rect2->size.width;
	int y1_min = rect1->top_left.y;
	int y1_max = y1_min + rect1->size.height;
	int y2_min = rect2->top_left.y;
	int y2_max = y2_min + rect2->size.height;

	bool intersection = x2_min > x1_max || y2_min > y1_max || y2_max < y1_min || x2_max < x1_min;
	if (intersection) {
		int xinter_min = max(x1_min, x2_min);
		int xinter_max = min(x1_max, x2_max);
		int yinter_min = max(y1_min, y2_min);
		int yinter_max = min(y1_max, y2_max);


		ei_point_t newPoint = {xinter_min, yinter_min};
		ei_size_t newSize = {xinter_max - xinter_min, yinter_max - yinter_min};
		ei_rect_t newRect = {newPoint, newSize};
		return newRect;
	}
	ei_point_t newPoint = {0, 0};
	ei_size_t newSize = {0, 0};
	ei_rect_t newRect = {newPoint, newSize};
	return newRect;

}
