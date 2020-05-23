//
// Created by devri on 15/05/2020.
//

#ifndef PROJETC_IG_UTILS_H
#define PROJETC_IG_UTILS_H
#include "ei_geometrymanager.h"
/**
 * @brief	Return if two strings are equal, mainly used to simplify code understanding
 */
int                     is_name_equal(ei_geometrymanager_name_t actual_name, ei_geometrymanager_name_t name);

/**
 * @brief	Do nothing, mainly used to simplify code understanding
 */
 void                   do_nothing();

/**
* @brief	Tell if an attribute is set by the developer, mainly used to simplify code understanding
*/
int                     is_defined(void *param);

/**
* @brief	Check if the allocation worked, print a message if there is a problem
*/
void                    *safe_malloc(size_t size);

/**
* @brief	Return the maximum between two int
*/
int                     max(int a, int b);


/**
 * @brief Make the intersection of 2 rectangle
 *
 * @param rect1		first rectangle
 * @param rect2		second rectangle
 *
 * @return		rectangle which is the intersection of the two others
 */
ei_rect_t inter_rect(const ei_rect_t* rect1, const ei_rect_t* rect2);


/**
 * @brief i		Make the copy of a rectangle and return a pointer to it
 *
 * @param rect		The rectangle which will be copied.
 *
 * @return		Copy of rect.
 */
ei_rect_t* copy_rect(ei_rect_t* rect);


/**
 * @brief		Return if two rectangle are egal
 *
 * @param rect1		First rectanle.
 * @param rect2		Second rectanle
 *
 * @return		return 0 or 1 if rectangles are equal.
 */
int is_equal(ei_rect_t* rect1, ei_rect_t* rect2);


/**
 * @brief		Make the fusion of two rectangle in a bigger one if
 *			it's needed
 *
 * @param rect1		First rectangle
 * @param rect2		Second rectangle
 *
 * @return		Pointer to the fusioned rectangle or NULL if a fusion
 *			is not appreciated.
 */
ei_rect_t* fusion_if(ei_rect_t* rect1, ei_rect_t* rect2);



/**
 * @brief 		Try to fusion the list at its maximum to not over displayed
 *			new  things on screen
 *
 * @param begin_pt	Pointer to the head of the rectangle linked list.
 */
void simplify_list(ei_linked_rect_t**  begin_pt);


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
ei_color_t ei_map_color (ei_surface_t surface, const uint32_t rgba);

#endif //PROJETC_IG_UTILS_H
