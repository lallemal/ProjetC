/******************************************************************************
* File:             utils.h
*
* Author:           Robin Bertin, Aymeric Devriésère, Louise Lallemand
* Created:          05/26/20
* Description:      Utils little functions which are essentials
*****************************************************************************/
#ifndef PROJETC_IG_EI_PLACER_H
#define PROJETC_IG_EI_PLACER_H
#include "ei_geometrymanager.h"

typedef struct ei_placer_t{
        ei_geometry_param_t     *manager;
        ei_anchor_t             anchor;
        int			x;
        int			y;
        int			width;
        int			height;
        float			rel_x;
        float			rel_y;
        float			rel_width;
        float			rel_height;
} ei_placer_t;

/**
 * @brief	Tells if a relative attribute is set
 *
 * @param	rel		The relative attribute
 *
 * @return      0 if not set, 1 otherwise
 */
int                     is_set_relative                 (float rel);
/**
 * @brief	Tells if the new and old screen location are different
 *
 * @param	r1		old screen location
 *
 * @param	r2		new screen location(after computation)
 *
 * @return      0 if not equal², 1 otherwise
 */
int                     are_old_and_new_diff            (ei_rect_t r1, ei_rect_t r2);
/**
 * @brief	Configure the content rect for the widget if it's a toplevel
 *
 * @param	widget		The widget to treat
 */
void                    special_case                    (struct ei_widget_t*	widget);
/**
 * @brief	Tells if the button is the default button of the top level
 *
 * @param	widget		The widget for which we have to determine if he is the close_button
 *
 * @return      0 if it's not, 1 otherwise
 */
int                     button_case                     (struct ei_widget_t*	widget);
/**
 * @brief	Setting defaults for the placer
 *
 * @param	to_set		The widget which need a setup for his placer
 */
void                    set_defaults_placer             (ei_widget_t *to_set);
/**
 * @brief	Placer's run function
 *
 * @param	widget		The widget to compute
 */
void                    ei_run_func                     (struct ei_widget_t*	widget);
/**
 * @brief	Placer's release function
 *
 * @param       widget          The widget recently free from the manager and on which computations has to be done
 */
void                    ei_release_func                 (struct ei_widget_t*	widget);
#endif //PROJETC_IG_EI_PLACER_H
