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


void			create_base_eventlist()
{
	ei_linked_event_t* for_keydown = calloc(1,sizeof(ei_linked_event_t));
	for_keydown->eventtype = ei_ev_keydown;
	ei_linked_event_t* for_keyup = calloc(1,sizeof(ei_linked_event_t));
	for_keyup->eventtype = ei_ev_keyup;
	for_keydown->next = for_keyup;
	ei_linked_event_t* for_mouseup = calloc(1,sizeof(ei_linked_event_t));
	for_mouseup->eventtype = ei_ev_mouse_buttonup;
	for_keyup->next = for_mouseup;
	ei_linked_event_t* for_mousedown = calloc(1,sizeof(ei_linked_event_t));
	for_mousedown->eventtype = ei_ev_mouse_buttondown;
	for_mouseup->next = for_mousedown;
	ei_linked_event_t* for_mousemove = calloc(1,sizeof(ei_linked_event_t));
	for_mousemove->eventtype = ei_ev_mouse_move;
	for_mousedown->next = for_mousemove;
	linked_event_head = for_keydown;
	linked_event_tail = for_mousemove;
}


void			destroy_base_eventlist()
{
	ei_linked_event_t* element = linked_event_head;
	while (element != NULL) {
		ei_linked_event_t* next = element->next;
		free(element);
		element = next;
	}
}


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
	ei_bool_t not_continue = EI_FALSE;
	while (tag_call != NULL) {
		// if the tag is all : do the callback
		if (strcmp(tag_call->tag, "all") == 0 ) {
			not_continue = tag_call->callback(NULL, &event, tag_call->user_param);
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
					not_continue = tag_call->callback(widget_cursor, &event, tag_call->user_param);

				}
			}
			// if it can't be localized, call the function.
			else {
				not_continue = tag_call->callback(NULL, &event, tag_call->user_param);
			}
		}
		// if the event is consumed
		if (not_continue) {
			return;
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
				not_continue = widget_call->callback(widget_call->widget, &event, widget_call->user_param);
			}
		}
		else {
			not_continue = widget_call->callback(NULL, &event, widget_call->user_param);
		}
		// if the event is consumed
		if (not_continue) {
			return;
		}
		widget_call = widget_call->next;
	}
}


void			add_to_listcall	(ei_linked_event_t*	list,
					ei_tag_t		tag,
					ei_widget_t*		widget,
					ei_callback_t		callback,
					void*			user_param)
{
	// If it is a tag to add
	if (widget == NULL) {
		ei_linked_tagcall_t* newElement = malloc(sizeof(ei_linked_tagcall_t));
		newElement->callback = callback;
		newElement->user_param = user_param;
		newElement->tag = tag;
		// If the list is empty
		if (list->tagcall_list == NULL) {
			newElement->next = NULL;
			list->tagcall_list = newElement;
			return;
		}
		newElement->next = list->tagcall_list;
		list->tagcall_list = newElement;
		return;
	}
	// If it is a widget to add
	ei_linked_widgetcall_t* newElement = malloc(sizeof(ei_linked_widgetcall_t));
	newElement->callback = callback;
	newElement->widget = widget;
	newElement->user_param = user_param;
	if (list->widgetcall_list == NULL) {
		newElement->next = NULL;
		list->widgetcall_list = newElement;
		return;
	}
	newElement->next = list->widgetcall_list;
	list->widgetcall_list = newElement;
}


// Tool to verify if bindings of an event by a tag are equals
int	isequal_tagcall(ei_linked_tagcall_t*	element,
			ei_tag_t		tag,
			ei_callback_t		callback)
{
	int tag_idem = strcmp(tag, element->tag) == 0;
	int callback_idem = callback == element->callback;
	return callback_idem && tag_idem;
}



// Tool to verify if bindings of an event by a widget are equals
int	isequal_widgetcall(ei_linked_widgetcall_t*	element,
			ei_widget_t*		widget,
			ei_callback_t		callback)
{
	int widget_idem = widget == element->widget;
	int callback_idem = callback == element->callback;
	return callback_idem && widget_idem;
}

void			del_to_listcall	(ei_linked_event_t*	list,
					ei_tag_t		tag,
					ei_widget_t*		widget,
					ei_callback_t		callback,
					void*			user_param)
{
	if (widget == NULL) {
		ei_linked_tagcall_t* cel = list->tagcall_list;
		if (cel == NULL) {
			return;
		}
		if (isequal_tagcall(cel, tag, callback)) {
			ei_linked_tagcall_t* next = cel->next;
			free(cel->user_param);
			free(cel);
			list->tagcall_list = next;
			return;
		}
		while (cel->next != NULL && isequal_tagcall(cel->next, tag, callback)) {
			cel = cel->next;
		}
		if (cel->next == NULL) {
			return;
		}
		ei_linked_tagcall_t* to_destroy = cel->next;
		cel->next = cel->next->next;
		free(to_destroy->tag);
		free(to_destroy->user_param);
		free(to_destroy);
		return;
	}
	ei_linked_widgetcall_t* cel = list->widgetcall_list;
	if (cel == NULL) {
		return;
	}
	if (isequal_widgetcall(cel, widget, callback)) {
		ei_linked_widgetcall_t* next = cel->next;
		free(cel->user_param);
		free(cel);
		list->widgetcall_list = next;
	}
	while (cel != NULL && isequal_widgetcall(cel->next, widget, callback)) {
		cel = cel->next;
	}
	if (cel == NULL) {
		return;
	}
	ei_linked_widgetcall_t* to_destroy = cel->next;
	cel->next = cel->next->next;
	free(to_destroy->user_param);
	free(to_destroy);
	return;
}
