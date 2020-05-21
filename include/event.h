/******************************************************************************
* File:             event.h
*
* Author:           Robin BERTIN (Nunwan)
* Created:          05/20/20
* Description:      
*****************************************************************************/
#ifndef EVENT_H
#define EVENT_H

#include "ei_event.h"

typedef struct ei_linked_tagcall {
	ei_tag_t tag;
	ei_callback_t callback;
	void* user_param;
	struct ei_linked_tagcall *next;
}ei_linked_tagcall_t;

typedef struct ei_linked_widgetcall {
	ei_widget_t* widget;
	ei_callback_t callback;
	void* user_param;
	struct ei_linked_widgetcall *next;
} ei_linked_widgetcall_t;

typedef struct ei_linked_event {
	ei_eventtype_t	eventtype;
	ei_linked_tagcall_t* tagcall_list;
	ei_linked_widgetcall_t* widgetcall_list;
	struct ei_linked_event *next;
} ei_linked_event_t;


/**
 * @brief		Retrieve the tag, widget and callback bind to the type
 *
 * @param eventtype	Type of the event to consider
 *
 * @return		The structure with tag list, widget list and callback associated.
 */
ei_linked_event_t*	retrieve_eventtype(ei_eventtype_t eventtype);

/**
 * @brief		Call all the callback which has to be call by the event
 *
 * @param list_todo	List of tag and callback and widget concerned
 */
void			call(ei_event_t event, ei_linked_event_t list_todo, ei_surface_t pick_surface);

/**
 * @brief		Return the widget under the cursor or NULL if it is not
 *			one if the correct name class
 *
 *
 * @param event		event considered
 * @param pick_surface  pick_surface to find the pick_color
 * @param tag		tag to match the widget under cursor and the name class
 *
 * @return		NULL if widget under cursor has not the class name tag
 *			the widget under cursor if it has the correct name class
 */
ei_widget_t*		on_widget	(ei_event_t		event,
					ei_surface_t		pick_surface,
					ei_tag_t		tag);


void			add_to_listcall	(ei_linked_event_t*	list,
					ei_tag_t		tag,
					ei_widget_t*		widget,
					ei_callback_t		callback,
					void*			user_param);

void			del_to_listcall	(ei_linked_event_t*	list,
					ei_tag_t		tag,
					ei_widget_t*		widget,
					ei_callback_t		callback,
					void*			user_param);

/* 
void			add_to_widgetcall	(ei_linked_event_t	list,
						ei_tag_t		tag,
						ei_callback_t		callback,
						void*			user_param);

void			del_to_widgetcall	(ei_linked_event_t	list,
						ei_tag_t		tag,
						ei_callback_t		callback,
						void*			user_param);

*/


#endif /* ifndef EVENT_H */
