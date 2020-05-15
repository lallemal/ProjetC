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
#endif //PROJETC_IG_UTILS_H
