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
#include "ei_utils.h"


#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)


uint32_t		ei_map_rgba		(ei_surface_t surface, const ei_color_t* color)
{
	int ir, ig, ib, ia;
	hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);
	uint32_t color_hex = 0;
	color_hex += color->red << (8*ir);
	color_hex += color->green << (8*ig);
	color_hex += color->blue << (8*ib);
	if (ia != -1) {
		color_hex += color->alpha << (8*ia);
	}
	return color_hex;
}


ei_color_t ei_map_color (ei_surface_t surface, const uint32_t rgba)
{
	int ir, ig, ib, ia;
	hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);
	ei_color_t color;
	uint32_t mask = 255;
	color.red = (rgba & (mask << 8*ir)) >> (8*ir);
	color.blue = (rgba & (mask << 8*ib)) >> (8*ib);
	color.green = (rgba & (mask << 8*ig)) >> (8*ig);
	if (ia != -1) {
		color.alpha = (rgba & (mask << 8*ia)) >> (8*ia);
	}
	else {
		color.alpha = 0;
	}
	return color;

}


void update_pixel	(ei_surface_t destination,
			uint32_t* dest_pt,
			ei_surface_t source,
			uint32_t* src_pt,
			ei_bool_t alpha)
{
	if (alpha == EI_FALSE) {
		*dest_pt = *src_pt;
	}
	else {
		ei_color_t color_dest = ei_map_color(destination, *dest_pt);
		ei_color_t color_src = ei_map_color(source, *src_pt);
		ei_color_t color_alpha;
		color_alpha.red = (color_src.alpha * color_src.red + (255 - color_src.alpha) * color_dest.red)/255;
		color_alpha.blue = (color_src.alpha * color_src.blue + (255 - color_src.alpha) * color_dest.blue)/255;
		color_alpha.green = (color_src.alpha * color_src.green + (255 - color_src.alpha) * color_dest.green)/255;
		*dest_pt = ei_map_rgba(destination, &color_alpha);
	}
}



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



int			ei_copy_surface		(ei_surface_t		destination,
						 const ei_rect_t*	dst_rect,
						 const ei_surface_t	source,
						 const ei_rect_t*	src_rect,
						 const ei_bool_t	alpha)
{
	int dest_x = dst_rect->top_left.x;
	int dest_y = dst_rect->top_left.y;
	int src_x = src_rect->top_left.x;
	int src_y = src_rect->top_left.y;
	for (int i=0; i < dst_rect->size.height; i++) {
		hw_surface_set_origin(destination, ei_point(dest_x, dest_y+i));
		hw_surface_set_origin(source, ei_point(src_x, src_y+i));
		uint32_t* dest_pt = (uint32_t *) hw_surface_get_buffer(destination);
		uint32_t* src_pt  = (uint32_t *) hw_surface_get_buffer(source);
		for (int j = 0; j < dst_rect->size.width; j++) {
			update_pixel(destination, dest_pt, source, src_pt, alpha);
			dest_pt++;
			src_pt++;
		}
	}
	//  Reset the origin of surface to 0,0
	hw_surface_set_origin(destination, ei_point_zero());
	hw_surface_set_origin(source, ei_point_zero());
}
