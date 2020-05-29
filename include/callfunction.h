/******************************************************************************
* File:             callfunction.h
*
* Author:           Robin Bertin, Aymeric Devriésère, Louise Lallemand
* Created:          05/22/20
* Description:      Functions for internals callback
*****************************************************************************/
#include "ei_event.h"
#include "ei_widget.h"

#ifndef CALLFUNCTION_H
#define CALLFUNCTION_H


/* In ei_buttonclass.c */
ei_bool_t button_on_release(ei_widget_t* widget, ei_event_t* event, void* user_param);

ei_bool_t button_on_press(ei_widget_t* widget, ei_event_t* event, void* user_param);

/* In ei_toplevel.c for closing, moving the toplevel and resize it */
ei_bool_t close_toplevel(ei_widget_t* widget, ei_event_t* event, void* user_param);

ei_bool_t move_top_up(ei_widget_t* widget, ei_event_t* event, void* user_param);

ei_bool_t move_top_onmove(ei_widget_t* widget, ei_event_t* event, void* user_param);

ei_bool_t move_top_down(ei_widget_t* widget, ei_event_t* event, void* user_param);

ei_bool_t dispatch_event(ei_widget_t* widget, ei_event_t* event, void* user_param);

ei_bool_t resize_top_onmove(ei_widget_t* widget, ei_event_t* event, void* user_param);

ei_bool_t resize_top_down(ei_widget_t* widget, ei_event_t* event, void* user_param);

ei_bool_t resize_top_up(ei_widget_t* widget, ei_event_t* event, void* user_param);

ei_bool_t process_radio(ei_widget_t* widget, ei_event_t* event, void* user_param);

#endif /* ifndef CALLFUNCTION_H */

