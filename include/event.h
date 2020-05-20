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
	ei_linked_tagcall_t tagcall_list;
	ei_linked_widgetcall_t widgetcall_list;
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
void			call(ei_linked_event_t list_todo);

/**
 * @brief		Return if the widget under the cursor or NULL if the event
 *			can't be positioned
 *
 * @param event		event considered
 *
 * @return		NULL if the event has no position or the widget under concerned.
 */
ei_widget_t*		on_widget(ei_event_t event);




#endif /* ifndef EVENT_H */
