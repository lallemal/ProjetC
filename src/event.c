/******************************************************************************
* File:             event.c
*
* Author:           Robin BERTIN (Nunwan)
* Created:          05/20/20
* Description:      
*****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "event.h"
#include "traverse_tools.h"
#include "ei_application.h"

// Static Variables of the linked lists of event.
static ei_linked_event_t* linked_event_head = NULL;
static ei_linked_event_t* linked_event_tail = NULL;


// Prototypes of tools functions

/**
 * @brief i			Retrive the rgba int below of pixel x,y
 *
 * @param pick_surface		Pick_surface where to find the pixel
 * @param x			x coordinate of the pixel
 * @param y			y coordinate of the pixel
 *
 * @return			integer on 32 bits which represent the pick_color
 */
uint32_t retrieve_color(ei_surface_t pick_surface, int x, int y);


/**
 * @brief		Give if a tag in a linked tagcall list is equal
 *			to tag + callback
 *
 * @param element	Element of linked tagcall list to test
 * @param tag		Tag to test the equality
 * @param callback	Callback to test the equality
 *
 * @return		1 or 0 : booleen to equality
 */
int	isequal_tagcall(ei_linked_tagcall_t*	element,
			ei_tag_t		tag,
			ei_callback_t		callback);


/**
 * @brief		Give if a widget in a linked widgetcall list is equal
 *			to widget + callback
 *
 * @param element	Element of linked tagcall list to test
 * @param tag		widget to test the equality
 * @param callback	Callback to test the equality
 *
 * @return		1 or 0 : booleen to equality
 */
int	isequal_widgetcall(ei_linked_widgetcall_t*	element,
			ei_widget_t*		widget,
			ei_callback_t		callback);



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
	linked_event_head = NULL;
	linked_event_tail = NULL;
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


uint32_t retrieve_color(ei_surface_t pick_surface, int x, int y)
{
	hw_surface_lock(pick_surface);
	uint32_t *color_rgba = (uint32_t *)hw_surface_get_buffer(pick_surface);
	ei_size_t size_pick = hw_surface_get_size(pick_surface);
	color_rgba += y * size_pick.width + x;
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
		if (widget_with_color != NULL && (strcmp(tag, widget_with_color->wclass->name) == 0 || strcmp(tag, "all") == 0)) {
			return widget_with_color;
		}
	}
	return NULL;
}


void			call(ei_event_t event, ei_linked_event_t list_todo, ei_surface_t pick_surface)
{
	// First , the tag list
	ei_linked_tagcall_t* tag_call = list_todo.tagcall_list;
	ei_bool_t not_continue = EI_FALSE;
	while (tag_call != NULL) {
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
		// if it can't be localized, call all the callback function.
		else {
			not_continue = call_widgets(ei_app_root_widget(), tag_call->tag, &event, tag_call->callback, tag_call->user_param);
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
			if (pick_color == widget_call->widget->pick_id) {
				not_continue = widget_call->callback(widget_call->widget, &event, widget_call->user_param);
			}
		}
		else {
			not_continue = widget_call->callback(widget_call->widget, &event, widget_call->user_param);
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



void			del_to_listcall	(ei_linked_event_t*	list,
					ei_tag_t		tag,
					ei_widget_t*		widget,
					ei_callback_t		callback,
					void*			user_param)
{
	if (widget != NULL && tag != NULL) {
		fprintf(stderr, "Both widget and tag can't be destroyed at the same time.\n");
	}
	if (widget == NULL) {
		ei_linked_tagcall_t* cel = list->tagcall_list;
		// if the list is empty
		if (cel == NULL) {
			return;
		}
		// If it is the first element
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
		// If the tag,callback is not in the list
		if (cel->next == NULL) {
			return;
		}
		ei_linked_tagcall_t* to_destroy = cel->next;
		cel->next = cel->next->next;
		free(to_destroy->tag);
		// free(to_destroy->user_param);
		free(to_destroy);
		return;
	}
	// Verify for the widget case.
	ei_linked_widgetcall_t* cel = list->widgetcall_list;
	if (cel == NULL) {
		return;
	}
	if (isequal_widgetcall(cel, widget, callback)) {
		ei_linked_widgetcall_t* next = cel->next;
		free(cel->user_param);
		free(cel);
		list->widgetcall_list = next;
		return;
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
