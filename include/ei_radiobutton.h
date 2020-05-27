//
// Created by aymeric on 27/05/2020.
//

#ifndef PROJETC_IG_EI_RADIOBUTTON_H
#define PROJETC_IG_EI_RADIOBUTTON_H
#include "ei_widget.h"
#include "stdio.h"
#include "stdlib.h"
#define MARGIN_LEFT 5
typedef struct ei_radio_button {
    ei_widget_t		        widget;
    ei_size_t		        requested_size;
    char**                      choices;
    char*			title;
    int                         nb_of_choices;
    ei_widget_t*               current_button;
    int                         title_height;
    int                         title_width;
} ei_radio_button;

void                    ei_radiobutton_configure                                (ei_widget_t      *widget,
                                                                                 ei_size_t       *requested_size,
                                                                                 char            ***choices,
                                                                                 char            **title,
                                                                                 int             *nb_of_choices);

void                    *ei_radiobutton_allofunc                                (void);

void                    ei_radiobutton_releasefunc                              (ei_widget_t* widget);

void                    ei_radiobutton_setdefaultsfunc                          (ei_widget_t* widget);

void                    ei_radiobutton_geomnotifyfunc                           (ei_widget_t* widget);

void                    ei_toplevel_drawfunc                                    (struct	ei_widget_t*	widget,
                                                                                 ei_surface_t	surface,
                                                                                 ei_surface_t	pick_surface,
                                                                                 ei_rect_t*	clipper);

void                    ei_radiobutton_register_class                            (void);
#endif //PROJETC_IG_EI_RADIOBUTTON_H
