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
#include "utils.h"


uint32_t ei_map_rgba(ei_surface_t surface, const ei_color_t* color)
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


/**
 * \brief	Converts a 32 bits integer into red, blue, green, alpha colors
 *		using the order of the channels of the surface.
 *
 * @param	surface		The surface where to store this pixel.
 * @param	rgba		The integer to convert.
 *
 * @return	ei_color_t : a color with the four channel.
 *
 */
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


/**
 * @brief Update the pixel on a surface with the source pixel. Combinate the two pixel if alpha
 * if enabled.
 *
 * @param destination	Destination surface which contains original pixel
 * @param dest_pt	pointer to the pixel on the destination surface.
 * @param source	Source surface which contains the new pixel to place
 * @param src_pt	pointer to the pixel on the source surface.
 * @param alpha		bool which indicate if source pixel has alpha.
 */
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
		// retrieve the R G B A colors channels
		ei_color_t color_dest = ei_map_color(destination, *dest_pt);
		ei_color_t color_src = ei_map_color(source, *src_pt);
		// Create the new color with alpha formula
		ei_color_t color_alpha;
		color_alpha.red = (color_src.alpha * color_src.red + (255 - color_src.alpha) * color_dest.red)/255;
		color_alpha.blue = (color_src.alpha * color_src.blue + (255 - color_src.alpha) * color_dest.blue)/255;
		color_alpha.green = (color_src.alpha * color_src.green + (255 - color_src.alpha) * color_dest.green)/255;
		// Change the destination pixel.
		*dest_pt = ei_map_rgba(destination, &color_alpha);
	}
}





void ei_draw_text       (ei_surface_t		surface,
			const ei_point_t*	where,
			const char*		text,
			const ei_font_t		font,
			ei_color_t		color,
			const ei_rect_t*	clipper)
{
	ei_surface_t text_surface = hw_text_create_surface(text, font, color);
	// Create the destination rectangle to fit with clipper & text
	ei_size_t text_size = hw_surface_get_size(text_surface);
	ei_rect_t text_rect = {*where, text_size};
	ei_rect_t destRect = inter_rect(clipper, &text_rect);
	// Lock the surface and copy it on the destination surface
	hw_surface_lock(text_surface);
	ei_copy_surface(surface, &destRect, text_surface, NULL, EI_TRUE);
	// Release the text_surface no longer needed
	hw_surface_unlock(text_surface);
	hw_surface_free(text_surface);
}


int ei_copy_surface    (ei_surface_t		destination,
			const ei_rect_t*	dst_rect,
			const ei_surface_t	source,
			const ei_rect_t*	src_rect,
			const ei_bool_t		alpha)
{
	// Calcul of the destination rect NULL or not
	int dest_x = 0;
	int dest_y = 0;
	if (dst_rect != NULL) {
		dest_x = dst_rect->top_left.x;
		dest_y = dst_rect->top_left.y;
	}
	int height_dest;
	int width_dest;
	ei_size_t destSurface_size = hw_surface_get_size(destination);
	if (dst_rect != NULL) {
		height_dest = dst_rect->size.height;
		width_dest = dst_rect->size.width;
	}
	else {
		height_dest = destSurface_size.height;
		width_dest = destSurface_size.width;
	}
	// Calcul of the source rect NULL or not
	int src_x = 0;
	int src_y = 0;
	if (src_rect != NULL) {
		int src_x = src_rect->top_left.x;
		int src_y = src_rect->top_left.y;
	}
	int height_src;
	int width_src;
	ei_size_t srcSurface_size = hw_surface_get_size(source);
	if (src_rect != NULL) {
		height_src = src_rect->size.height;
		width_src = src_rect->size.width;
	}
	else {
		height_src = srcSurface_size.height;
		width_src = srcSurface_size.width;
	}
	// Verify the dimension.
	if (height_src > height_dest || width_src > width_dest) {
		return EXIT_FAILURE;
	}
	// Take the both pointers for the copy of each pixels
	uint32_t* dest_pt = (uint32_t *) hw_surface_get_buffer(destination);
	uint32_t* src_pt  = (uint32_t *) hw_surface_get_buffer(source);
	// point the origin of surfaces on the first line
	dest_pt += dest_x + dest_y * destSurface_size.width;
	src_pt += src_x + src_y * srcSurface_size.width;
	for (int i=0; i < height_dest; i++) {
		for (int j = 0; j < width_dest; j++) {
			update_pixel(destination, dest_pt, source, src_pt, alpha);
			// Increment the pointer to continue the line
			dest_pt++;
			src_pt++;
		}
		// Put both pointers to the next beginning line
		dest_pt += destSurface_size.width - (dest_x + width_dest) + dest_x;
		src_pt += srcSurface_size.width - (src_x + width_dest) + src_x;
	}
	return EXIT_SUCCESS;
}



void			ei_fill			(ei_surface_t		surface,
						 const ei_color_t*	color,
						 const ei_rect_t*	clipper)
{
	uint32_t color_rgba = ei_map_rgba(surface, color);
	ei_rect_t rect_surface = hw_surface_get_rect(surface);
	// if clipper is NULL, fill the entire surface with the color
	if (clipper == NULL) {
		 uint32_t* pixel_ptr = (uint32_t *)hw_surface_get_buffer(surface);
		 for (int i = 0; i < rect_surface.size.width * rect_surface.size.height; i++)
			 *pixel_ptr++ = color_rgba;
	}
	else {
		int dest_x = clipper->top_left.x;
		int dest_y = clipper->top_left.y;
		uint32_t* dest_pt = (uint32_t *) hw_surface_get_buffer(surface);
		// go to the top left of the clipper
		dest_pt += dest_x + dest_y * rect_surface.size.width;
		for (int i=0; i < clipper->size.height; i++) {
			for (int j = 0; j < clipper->size.width; j++) {
				 *dest_pt++ = color_rgba;
			}
			// go to the next beginning of line of the clipper
			dest_pt += rect_surface.size.width - (dest_x + clipper->size.width) + dest_x;
		}
	}
}
