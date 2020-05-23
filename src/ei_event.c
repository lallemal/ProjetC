/******************************************************************************
* File:             ei_event.c
*
* Author:           Robin BERTIN (Nunwan)
* Created:          05/20/20
* Description:
*****************************************************************************/
#include <stdio.h>

#include "ei_event.h"
#include "event.h"


void		ei_bind			(ei_eventtype_t		eventtype,
					 ei_widget_t*		widget,
					 ei_tag_t		tag,
					 ei_callback_t		callback,
					 void*			user_param)
{
	ei_linked_event_t* to_consider = retrieve_eventtype(eventtype);
	// if widget AND tag are NULL : throw an error and stop
	if (widget == NULL && tag == NULL) {
		fprintf(stderr, "Both widget and tag can't be NULL at \
				the same time in a binding \n");
		return;
	}
	add_to_listcall(to_consider, tag, widget, callback, user_param);
}


void		ei_unbind		(ei_eventtype_t		eventtype,
					 ei_widget_t*		widget,
					 ei_tag_t		tag,
					 ei_callback_t		callback,
					 void*			user_param)
{
	ei_linked_event_t* to_consider = retrieve_eventtype(eventtype);
	// if widget AND tag are NULL : throw an error and stop
	if (widget == NULL && tag == NULL) {
		fprintf(stderr, "Both widget and tag can't be NULL at \
				the same time in a unbinding \n");
		return;
	}
	del_to_listcall(to_consider, tag, widget, callback, user_param);
}
