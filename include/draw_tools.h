/******************************************************************************
* File:             draw_tools.h
*
* Author:           Robin BERTIN (Nunwan)
* Created:          05/15/20
* Description:
*****************************************************************************/

#ifndef DRAW_H
#define DRAW_H

#include "ei_widgetclass.h"
#include "ei_widget.h"
#include "ei_draw.h"

/**
 * @brief	Fonction to know where is the point at the top right corner in the rectangle
 *
 * @param       surface         Global surface of the frame (in the case where clipper is NULL => rect_to_fill is NULL)
 * @param	rect		Global rectangle which will contain the text or the image
 * @param	anchor		The anchor link to the text or the image.
 * @param	width_text	width of the text or the image
 * @param	height_text	height of the text or the image
 *
 * @return			The point at the top right of the text or the image
 */
ei_point_t* anchor_point(ei_surface_t surface, ei_rect_t* rect, ei_anchor_t anchor, int width_text, int height_text);


/**
 * @brief	Fonction which give a clearer color fr the relief
 *
 * @param       color         actual color of the frame
 *
 * @return		        a clearer color of the actual color of the frame
 */

ei_color_t clear_color(ei_color_t color);

/**
 * @brief	Fonction which give a darker color fr the relief
 *
 * @param       color         actual color of the frame
 *
 * @return		        a darker color of the actual color of the frame
 */

ei_color_t dark_color(ei_color_t color);



/**
 * @brief	Fonction which draw up and down of the rect to make a relief.
 *
 * @param       rect_to_fill    rectangle in whiwh there is the separatition in two colors to make the relief
 * @param	surface		Global surface of the frame
 * @param	frame		frame which has relief
 * @param	clipper 	clipper of the rect_to_fill
 * @param	clear_up	booleen which means if we want the clearer color on the top (depends on the value of relief)
 *
 */
void draw_up_and_down_relief(ei_rect_t* rect_to_fill, ei_surface_t surface, ei_color_t color, ei_rect_t* clipper, ei_bool_t clear_up);

#endif /* ifndef DRAW_H */