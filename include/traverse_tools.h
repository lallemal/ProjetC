/******************************************************************************
* File:             traverse_tools.h
*
* Author:           Robin BERTIN (Nunwan)
* Created:          05/15/20
* Description:      
*****************************************************************************/
#ifndef TRAVERSE_H
#define TRAVERSE_H

#include "ei_widget.h"


/**
 * @brief		Iterate to the next_sibling widget and point widget_pt on it.
 *
 * @param widget_pt	Pointer to a widget that we traverse.
 */
void next_sibling_widget(ei_widget_t** widget_pt);


/**
 * @brief		Traverse children of widget and destroy them.
 *
 * @param begin		The first widget that has to be destroyed with
 *			all of its children.
 */
void destroy_widgets(ei_widget_t* begin);


/**
 * @brief		Traverse all the widgetclass and destroy them.
 *
 * param begin		The first widgetclass that has to be destroyed with
 *			all of its children.
 */
void destroy_widgetclass(ei_widgetclass_t* begin);



/**
 * @brief		Traverse all the widget and draw them. 
 *
 * @param begin		The first widget to be drawn and its children.
 * @param surface	The surface on which widget has to be drawn.
 * @param pick_surface
 *
 */
void draw_widgets      (ei_widget_t* begin,
			ei_surface_t surface,
			ei_surface_t pick_surface);

#endif /* ifndef TRAVERSE_H */
