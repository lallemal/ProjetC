//
// Created by devri on 15/05/2020.
//
#include <stdbool.h>

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "utils.h"
#include "ei_utils.h"

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
	if (!intersection) {
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


ei_rect_t* copy_rect(ei_rect_t* rect)
{
	ei_rect_t* newRect = malloc(sizeof(ei_rect_t));
	newRect->size.height = rect->size.height;
	newRect->size.width = rect->size.width;
	newRect->top_left.x = rect->top_left.x;
	newRect->top_left.y = rect->top_left.y;
	return newRect;
}


int is_equal(ei_rect_t* rect1, ei_rect_t* rect2)
{
	int x_idem = rect1->top_left.x == rect2->top_left.x;
	int y_idem = rect1->top_left.y == rect2->top_left.y;
	int height_idem = rect1->size.height == rect2->size.height;
	int width_idem = rect1->size.width == rect2->size.width;
	return x_idem && y_idem && height_idem && width_idem;
}


ei_rect_t* fusion_if(ei_rect_t* rect1, ei_rect_t* rect2)
{
	ei_rect_t inter = inter_rect(rect1, rect2);
	int h1 = rect1->size.height;
	int h2 = rect2->size.height;
	int h3 = inter.size.height;
	int w1 = rect1->size.width;
	int w2 = rect2->size.width;
	int w3 = inter.size.width;
	//if (is_equal(rect1, &inter)) {
	//	return copy_rect(rect2);
	//}
	//if (is_equal(rect2, &inter)) {
	//	return copy_rect(rect1);
	//}
	if ((h1*w1) + (h2*w2) > (h1 + h2 - h3) * (w1 + w2 - w3)) {
		return NULL;
	}
	else {
		int x1 = rect1->top_left.x;
		int x2 = rect2->top_left.x;
		int y1 = rect1->top_left.y;
		int y2 = rect2->top_left.y;
		ei_rect_t newRect = ei_rect(ei_point(min(x1, x2), min(y1, y2)), ei_size(w1 + w2 - w3, h1 + h2 - h3));
		return copy_rect(&newRect);
	}
}


void destroy_linked_rect(ei_linked_rect_t* to_destroy, ei_linked_rect_t**  head_pt)
{
	ei_linked_rect_t* element = *head_pt;
	if (element == to_destroy) {
		*head_pt = element->next;
		free(to_destroy);
		return;
	}
	while (element != NULL && element->next != to_destroy) {
		element = element->next;
	}
	if (element == NULL) {
		return;
	}
	if (to_destroy->next == NULL) {
		element->next = NULL;
		free(to_destroy);
		return;
	}
	element->next = to_destroy->next;
	free(to_destroy);
}


void simplify_list(ei_linked_rect_t**  begin_pt)
{
	ei_linked_rect_t* element = *begin_pt;
	int b = 0;
	while (element != NULL) {
		ei_linked_rect_t* to_try = element->next;
		while (to_try != NULL) {
			ei_rect_t* fusionned = fusion_if(&(element->rect), &(to_try->rect));
			if (fusionned == NULL) {
				to_try = to_try->next;
			}
			else {
				ei_rect_t* old = &(element->rect);
				element->rect = *fusionned;
				free(old);
				destroy_linked_rect(to_try, begin_pt);
				b = 1;
				break;

			}
		}
		if (b == 1) {
			break;
		}
		else {
			element = element->next;
		}
	}
	if (b == 1) {
		simplify_list(begin_pt);
	}
}
