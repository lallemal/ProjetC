//
// Created by aymeric on 19/05/2020.
//

#ifndef PROJETC_IG_EI_TOPLEVEL_H
#define PROJETC_IG_EI_TOPLEVEL_H
#include "ei_types.h"
#include "ei_widget.h"
typedef struct ei_toplevel {
    ei_widget_t	        widget;

    int                 is_closable;
    ei_axis_set_t       resize_params;
    char*		title   ;
    ei_size_t           min_size;
} ei_toplevel;

void                    *ei_toplevel_allofunc                            (void);

void                    ei_toplevel_releasefunc                         (ei_widget_t*	widget);

void                    ei_toplevel_setdefaultsfunc                     (ei_widget_t* widget);

void                    ei_toplevel_geomnotifyfunc                      (struct ei_widget_t* widget);

void                    ei_toplevel_drawfunc                            (struct	ei_widget_t*	widget,
                                                                        ei_surface_t	surface,
                                                                        ei_surface_t	pick_surface,
                                                                        ei_rect_t*	clipper);


void                    ei_toplevel_register_class                      (void);
#endif //PROJETC_IG_EI_TOPLEVEL_H
