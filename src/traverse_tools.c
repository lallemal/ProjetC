/******************************************************************************
* File:             traverse_tools.c
*
* Author:           Robin Bertin, Aymeric Devriésère, Louise Lallemand
* Created:          05/15/20
* Description:      
*****************************************************************************/
#include <stdlib.h>
#include <string.h>

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
		ei_widget_t* widget_color = find_with_color(child, pick_color, pick_surface);
		if ( widget_color != NULL) {
			return widget_color;
		}
	}
	return NULL;
}


ei_bool_t call_widgets(ei_widget_t* begin, ei_tag_t tag, ei_event_t* event, ei_callback_t callback, void* user_param)
{
	ei_widget_t* child = begin->children_head;
	ei_bool_t not_continue;
	if (strcmp(tag, "all") == 0 || strcmp(tag, begin->wclass->name) == 0) {
		not_continue = callback(begin, event, user_param);
	}
	if (!not_continue) {
		for (child; child!= NULL; next_sibling_widget(&child)) {
			not_continue = call_widgets(child, tag, event, callback, user_param);
			if (not_continue) {
				break;
			}
		}
	}
	return not_continue;
}
