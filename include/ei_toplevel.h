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

#endif //PROJETC_IG_EI_TOPLEVEL_H
