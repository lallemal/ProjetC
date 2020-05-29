/******************************************************************************
* File:             utils.c
*
* Author:           Robin Bertin, Aymeric Devriésère, Louise Lallemand
* Created:          05/26/20
* Description:      
*****************************************************************************/
#include <stdbool.h>

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "utils.h"
#include "ei_utils.h"

#define min(a,b) (a<=b?a:b)

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
	if (is_equal(rect1, &inter)) {
		return copy_rect(rect2);
	}
	if (is_equal(rect2, &inter)) {
		return copy_rect(rect1);
	}
	int h1 = rect1->size.height;
	int h2 = rect2->size.height;
	int h3 = inter.size.height;
	int w1 = rect1->size.width;
	int w2 = rect2->size.width;
	int w3 = inter.size.width;

	int x1 = rect1->top_left.x;
	int x2 = rect2->top_left.x;
	int x3 = inter.top_left.x;
	int y1 = rect1->top_left.y;
	int y2 = rect2->top_left.y;
	int y3 = inter.top_left.y;
	int solu_base = h1*w1 + h2*w2;
	ei_point_t topleft = ei_point(min(x1, x2), min(y1, y2));
	ei_size_t newsize = ei_size(max(x1+w1, x2+w2) - topleft.x, max(y1 + h1, y2 + h2) - topleft.y);
	int quadrant = (newsize.width)* (newsize.height);
	if (quadrant < 1.5 * solu_base) {
		ei_point_t topleft = ei_point(min(x1, x2), min(y1, y2));
		ei_size_t newsize = ei_size(max(x1+w1, x2+w2) - topleft.x, max(y1 + h1, y2 + h2) - topleft.y);
		ei_rect_t newRect = ei_rect(topleft, newsize);
		return copy_rect(&newRect);
	}
	return NULL;
}


void destroy_linked_rect(ei_linked_rect_t* to_destroy, ei_linked_rect_t**  head_pt, ei_linked_rect_t** tail_pt)
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
		*tail_pt = element;
		free(to_destroy);
		return;
	}
	element->next = to_destroy->next;
	free(to_destroy);
}


void simplify_list(ei_linked_rect_t**  begin_pt, ei_linked_rect_t** tail_pt)
{
	ei_linked_rect_t* element = *begin_pt;
	int b = 0;
	// Double traverse of the list with a element and the other following it
	while (element != NULL) {
		ei_linked_rect_t* to_try = element->next;
		while (to_try != NULL) {
			// If a fusion is needed : fusion else continue
			ei_rect_t* fusionned = fusion_if(&(element->rect), &(to_try->rect));
			if (fusionned == NULL) {
				to_try = to_try->next;
			}
			else {
				element->rect = *fusionned;
				free(fusionned);
				destroy_linked_rect(to_try, begin_pt, tail_pt);
				b = 1;
				break;

			}
		}
		// If a fusion was done, stop and redo
		if (b == 1) {
			break;
		}
		else {
			element = element->next;
		}
	}
	// Redo the simplification with the new list.
	if (b == 1) {
		simplify_list(begin_pt, tail_pt);
	}
}

