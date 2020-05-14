/******************************************************************************
* File:             ei_draw.c
*
* Author:           Robin BERTIN (Nunwan)
* Created:          05/14/20
* Description:      
*****************************************************************************/
#include <stdlib.h>

#include "ei_draw.h"
#include "hw_interface.h"


#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)


ei_rect_t inter_rect(const ei_rect_t* rect1, const ei_rect_t* rect2)
{
	int x1_min = rect1->top_left.x;
	int x1_max = x1_min + rect1->size.width;
	int x2_min = rect2->top_left.x;
	int x2_max = x2_min + rect2->size.width;
	int y1_min = rect1->top_left.y;
	int y1_max = y1_min + rect1->size.height;
	int y2_min = rect2->top_left.y;
	int y2_max = y2_min + rect2->size.height;

	int xinter_min = max(x1_min, x2_min);
	int xinter_max = min(x1_max, x2_max);
	int yinter_min = max(y1_min, y2_min);
	int yinter_max = min(y1_max, y2_max);


	ei_point_t newPoint = {xinter_min, yinter_min};
	ei_size_t newSize = {xinter_max - xinter_min, yinter_max - yinter_min};
	ei_rect_t newRect = {newPoint, newSize};
	return newRect;
}





void			ei_draw_text		(ei_surface_t		surface,
						 const ei_point_t*	where,
						 const char*		text,
						 const ei_font_t	font,
						 ei_color_t		color,
						 const ei_rect_t*	clipper)
{
	ei_surface_t text_surface = hw_text_create_surface(text, font, color);
	ei_size_t text_size = hw_surface_get_size(text_surface);
	ei_rect_t text_rect = {*where, text_size};
	ei_rect_t destRect = inter_rect(clipper, &text_rect);
	ei_copy_surface(surface, &destRect, text_surface, NULL, EI_FALSE);
}
