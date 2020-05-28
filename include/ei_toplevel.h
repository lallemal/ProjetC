/******************************************************************************
* File:             ei_toplevel.h
*
* Author:           Robin Bertin, Aymeric Devriésère, Louise Lallemand
* Created:          05/26/20
* Description:      
*****************************************************************************/

#ifndef PROJETC_IG_EI_TOPLEVEL_H
#define PROJETC_IG_EI_TOPLEVEL_H
#include "ei_types.h"
#include "ei_widget.h"
#define MARGIN_TOP 5
#define MARGIN_LEFT 10
#define LOW_PROC 0
typedef struct ei_toplevel {
        ei_widget_t		        widget;
        ei_size_t		        requested_size;
        ei_color_t		        color;
        ei_bool_t		        closable;
        ei_axis_set_t		        resizable;
        ei_size_t*  	                min_size;
        ei_widget_t                     *sub_frame;
        ei_widget_t                     *resize_tool;
        ei_widget_t                     *close_button;
        char*			        title;
        int 			        border_width;
        int                             title_width;
        int                             title_height;


} ei_toplevel;

void                    *ei_toplevel_allofunc                            (void);

void                    ei_toplevel_releasefunc                         (ei_widget_t*	widget);

void                    ei_toplevel_setdefaultsfunc                     (ei_widget_t* widget);

void                    ei_toplevel_geomnotifyfunc                      (struct ei_widget_t* widget);

void                    ei_toplevel_drawfunc                            (struct	ei_widget_t*	widget,
                                                                         ei_surface_t	surface,
                                                                         ei_surface_t	pick_surface,
                                                                         ei_rect_t*	clipper);

/**
 * @brief	Initialise the subtools of a toplevel like the sub_buttons
 *
 * @param	toplevel	The toplevel for which the subtools have to be initialized
 */
void                    init_the_sub_tools                              (ei_toplevel *toplevel);

/**
 * @brief	Configure the subttols of a toplevel, for exemple set the content rect to the good dimension, places the close button ...
 *
 * @param	to_configure	The toplevel who needs to be configured
 */
void                    configure_sub_part                              (ei_toplevel *to_configure);
/**
 * @brief	Draw the border of the toplevel
 *
 * @param	surface         The surface where the border will be draw
 * @param	pick_surface    The pick_surface where to draw the borders too
 * @param	draw_rect       The rectangle of the widget minus the header
 * @param	widget          The toplevel widget
 * @param	clipper         The actual clipper
 */
void                    draw_border                                      (ei_surface_t	surface,
                                                                         ei_surface_t	pick_surface,
                                                                         ei_rect_t*     draw_rect,
                                                                         ei_widget_t*	widget,
                                                                         ei_rect_t*     clipper);
/**
 * @brief	Compute the size of the title of the toplevel and truncate it if it's too long
 *
 * @param	to_draw         The concerned toplevel
 * @param	text_placer     The biggest rectangle which can contain the text
 * @param	color           The wished color of the title
 * @param	surface         The surface on which the text will be draw
 * @param	clipper         The actual clipper
 */
void                    compute_text_size                               (ei_toplevel *to_draw,
                                                                         ei_rect_t *text_placer,
                                                                         ei_color_t *color,
                                                                         ei_surface_t *surface,
                                                                         ei_rect_t *clipper)
#endif //PROJETC_IG_EI_TOPLEVEL_H
