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
#include "ei_application.h"

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


ei_widget_t*		on_widget	(ei_event_t		event,
					ei_surface_t		pick_surface,
					ei_tag_t		tag)
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
		ei_widget_t* widget_with_color = find_with_color(ei_app_root_widget(), pick_color, pick_surface);
		return widget_with_color;
	}
	return NULL;
}


void			call(ei_event_t event, ei_linked_event_t list_todo, ei_surface_t pick_surface)
{
	// First , the tag list
	ei_linked_tagcall_t* tag_call = list_todo.tagcall_list;
	while (tag_call != NULL) {
		// if the tag is all : do the callback
		if (strcmp(tag_call->tag, "all") == 0 ) {
			tag_call->callback(NULL, &event, tag_call->user_param);
		}
		else {
			int is_buttonup = event.type == ei_ev_mouse_buttonup;
			int is_buttondown = event.type == ei_ev_mouse_buttondown;
			int is_move = event.type == ei_ev_mouse_move;
			// Verify if the event has location attributes.
			if (is_move || is_buttonup || is_buttondown) {
				ei_widget_t* widget_cursor = on_widget(event, pick_surface, tag_call->tag);
				// if there is a widget of class _tag_ under cursor
				if (widget_cursor != NULL) {
					tag_call->callback(widget_cursor, &event, tag_call->user_param);

				}
			}
			// if it can't be localized, call the function.
			else {
				tag_call->callback(NULL, &event, tag_call->user_param);
			}
				
		}
		tag_call = tag_call->next;
	}
	// Second : the widget list.
	ei_linked_widgetcall_t* widget_call = list_todo.widgetcall_list;
	while (widget_call != NULL) {
		int is_buttonup = event.type == ei_ev_mouse_buttonup;
		int is_buttondown = event.type == ei_ev_mouse_buttondown;
		int is_move = event.type == ei_ev_mouse_move;
		// Verify if the event has location attributes.
		if (is_move || is_buttonup || is_buttondown) {
			int x_cursor = event.param.mouse.where.x;
			int y_cursor = event.param.mouse.where.y;
			uint32_t pick_color = retrieve_color(pick_surface, x_cursor, y_cursor);
			// Verify the pick color to know if the widget is under cursor
			if (pick_color == ei_map_rgba(pick_surface, widget_call->widget->pick_color)) {
				widget_call->callback(widget_call->widget, &event, widget_call->user_param);
			}
		}
		else {
			widget_call->callback(NULL, &event, tag_call->user_param);
		}
		widget_call = widget_call->next;
	}
}
