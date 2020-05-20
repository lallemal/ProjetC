/******************************************************************************
* File:             event.c
*
* Author:           Robin BERTIN (Nunwan)
* Created:          05/20/20
* Description:      
*****************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "traverse_tools.h"

static ei_linked_event_t* linked_event_head = NULL;
static ei_linked_event_t* linked_event_tail = NULL;

ei_linked_event_t*	retrieve_eventtype(ei_eventtype_t eventtype)
{
	ei_linked_event_t* element = linked_event_head;
	while (element != NULL) {
		if (element->eventtype == eventtype) {
			return element;
		}
		element = element->next;
	}
	return NULL;
}


/**
 * @brief i			Retrive the rgba int below of pixel x,y
 *
 * @param pick_surface		Pick_surface where to find the pixel
 * @param x			x coordinate of the pixel
 * @param y			y coordinate of the pixel
 *
 * @return			integer on 32 bits which represent the pick_color
 */
uint32_t retrieve_color(ei_surface_t pick_surface, int x, int y)
{
	hw_surface_lock(pick_surface);
	uint32_t *color_rgba = (uint32_t *)hw_surface_get_buffer(pick_surface);
	uint32_t pick_color = *color_rgba;
	hw_surface_unlock(pick_surface);
	return pick_color;
}


int			on_widget	(ei_event_t		event,
					ei_surface_t		pick_surface,
					ei_tag_t		tag,
					ei_widget_t*		widget)
{
	int is_buttonup = event.type == ei_ev_mouse_buttonup;
	int is_buttondown = event.type == ei_ev_mouse_buttondown;
	int is_move = event.type == ei_ev_mouse_move;
	// Verify if the widget is the good one only if the event can be localized
	if (is_move || is_buttonup || is_buttondown) {
		int x_mouse = event.param.mouse.where.x;
		int y_mouse = event.param.mouse.where.y;
		uint32_t pick_color = retrieve_color(pick_surface, x_mouse, y_mouse);
		// If the event is linked to a tag
		if (widget == NULL) {
			// If the tag is all, it's always OK
			if (strcmp(tag, "all") == 0) {
				return 1;
			}
			ei_widget_t* widget_with_color = find_with_color(pick_color, pick_surface);
			// compare the class name and the tag
			if (strcmp(widget_with_color->wclass->name, tag) == 0) {
				return 1;
			}
			// If no one is OK return not OK
			return 0;
		}
		// If the event is linked to a widget
		else {
			// Verify is the pick color is the same
			if (pick_color == ei_map_rgba(pick_surface, widget->pick_color)) {
				return 1;
			}
			return 0;
		}
	}
	return 1;
}
