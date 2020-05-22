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
#include "ei_types.h"

/**
 * @brief	Fonction to know where is the point at the top right corner in the rectangle
 *
 * @param	rect		Global rectangle which will contain the text or the image
 * @param	anchor		The anchor link to the text or the image.
 * @param	width_text	width of the text or the image
 * @param	height_text	height of the text or the image
 *
 * @return			The point at the top right of the text or the image
 */
ei_point_t* anchor_point( ei_rect_t* rect, ei_anchor_t anchor, int width_text, int height_text);


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
 * @brief	Fonction which draw up of the rect to make a relief.
 *
 * @param       rect_to_fill    rectangle in whiwh there is the separatition in two colors to make the relief
 * @param	surface		Global surface of the frame
 * @param	frame		frame which has relief
 * @param	clipper 	clipper of the rect_to_fill
 * @param	clear_up	booleen which means if we want the clearer color on the top (depends on the value of relief)
 *
 */
void draw_up_relief(ei_rect_t* rect_to_fill, ei_surface_t surface, ei_color_t color, ei_bool_t clear_up, ei_rect_t* clipper);

/**
 * @brief	Fonction which draw down of the rect to make a relief.
 *
 * @param       rect_to_fill    rectangle in whiwh there is the separatition in two colors to make the relief
 * @param	surface		Global surface of the frame
 * @param	frame		frame which has relief
 * @param	clipper 	clipper of the rect_to_fill
 * @param	clear_up	booleen which means if we want the clearer color on the top (depends on the value of relief)
 *
 */
void draw_down_relief(ei_rect_t* rect_to_fill, ei_surface_t surface, ei_color_t color, ei_bool_t clear_up, ei_rect_t* clipper);

/**
 * @brief	Fonction which draw the text
 *
 * @param       text            text to draw
 * @param	text_font       font of the text to draw
 * @param	rect_to_fill	the rectangle in which we want to draw the text
 * @param	text_anchor 	anchor of the text
 * @param	surface	        surface concerned by the drawing
 * @param	text_color 	color of the text
 * @param	clipper	        clipper of the frame
 *
 */

void draw_text(char* text, ei_font_t text_font, ei_rect_t* rect_to_fill, ei_anchor_t text_anchor, ei_surface_t surface, ei_color_t text_color, ei_rect_t* clipper);

/**
 * @brief	Fonction which draw the image
 *
 * @param       image           image to draw
 * @param	rect_to_fill	the rectangle in which we want to draw the image
 * @param	img_anchor 	anchor of the image
 * @param	img_rect 	rectangle of the image
 * @param	surface	        surface concerned by the drawing
 * @param	clipper	        clipper of the frame
 *
 */

void draw_image(ei_surface_t image, ei_rect_t* rect_to_fill, ei_anchor_t img_anchor, ei_rect_t* img_rect, ei_rect_t* clipper, ei_surface_t surface);

/**
 * @brief	Fonction which return the sign of the number
 *
 * @param       nombre         number which we want to know the sign
 *
 * @return		       -1 if the number is negative, 1 if the number is positive
 */

int signe_inverse(float nombre);

/**
 * @brief	Fonction which create an arc of circle.
 *
 * @param       center         point which is the center of the circle
 * @param       radius         int which is the radius of the circle
 * @param       corner_begin   corner of the beginning of the arc
 * @param       corner_end     corner of the end of the arc
 *
 * @return		       a linked point list which represents the arc
 */

ei_linked_point_t* arc_point(ei_point_t center, int radius, float corner_begin, float corner_end);

/**
 * @brief	Fonction which draw a rectangle with rounded corner
 *
 * @param       rect           rectangle at the beggining which will have rounded corner
 * @param       radius         int which is the radius of the arc of the rounded corners
 * @param       part           char which means if we want up, down or the totality of the rectangle with rounded corners
 *
 * @return		       a linked point list which represents the rectangle with rounded corners
 */

ei_linked_point_t* rounded_frame(ei_rect_t* rect, int radius, int part);

/**
 * @brief	Fonction which draw the button
 *
 * @param       surface        surface where we will draw the button
 * @param       rect_button    the rectangle in which we must to draw the button
 * @param       color          color of the button
 * @param       border_width   border of the button
 * @param       corner_radius  radius for the rounded corners
 * @param       relief         relief of the button
 *
 */

void draw_button(ei_surface_t surface, ei_rect_t* rect_button, ei_color_t color, int border_width, int corner_radius, ei_relief_t relief, ei_rect_t* clipper);


#endif /* ifndef DRAW_H */