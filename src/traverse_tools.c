/******************************************************************************
* File:             traverse_tools.c
*
* Author:           Robin BERTIN (Nunwan)
* Created:          05/15/20
* Description:      
*****************************************************************************/
#include <stdlib.h>
#include "traverse_tools.h"
#include "ei_geometrymanager.h"
#include "ei_widget.h"


/* Point widget_pt to the next sibling of *widget_pt.
 * Must be called when all the resources are not freed
 **/
void next_sibling_widget(ei_widget_t** widget_pt)
{
	*widget_pt = (*widget_pt)->next_sibling;
}


void destroy_widgets(ei_widget_t* begin)
{
	ei_widget_t* child = begin->children_head;
	while (child != NULL) {
		ei_widget_t* nextChild = child->next_sibling;
		ei_widget_destroy(child);
		child = nextChild;
	}
}


void destroy_widgetclass(ei_widgetclass_t* begin)
{
	ei_widgetclass_t* next = begin->next;
	free(begin);
	if (next != NULL) {

		destroy_widgetclass(next);
	}
}



void draw_widgets      (ei_widget_t*	begin,
			ei_surface_t	surface,
			ei_surface_t	pick_surface,
			ei_rect_t	clipper)
{
	ei_widget_t* child = begin->children_head;

	begin->wclass->drawfunc(begin, surface, pick_surface, &clipper);
	// draw all the child and their child recursively
	for (child; child != NULL; next_sibling_widget(&child)) {
		draw_widgets(child, surface, pick_surface, clipper);
	}
}


void free_linked_rect(ei_linked_rect_t* begin)
{
	if (begin != NULL) {
		ei_linked_rect_t* next = begin->next;
		free(begin);
		free_linked_rect(next);
	}
}


ei_widget_t* find_with_color(ei_widget_t* begin, uint32_t pick_color, ei_surface_t pick_surface)
{
	ei_widget_t *child = begin->children_head;
	if (begin->pick_id == pick_color) {
		return begin;
	}
	for (child; child != NULL; next_sibling_widget(&child)) {
		if (find_with_color(child, pick_color, pick_surface) != NULL) {
			return child;
		}
	}
	return NULL;
}
