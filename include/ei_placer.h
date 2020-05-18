//
// Created by devri on 13/05/2020.
//

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
 * @brief	Tells if a relative attribute us set
 */
int                     is_set_relative                 (float rel);

int                     are_old_and_new_diff            (ei_rect_t r1, ei_rect_t r2);

void                    ei_run_func                     (struct ei_widget_t*	widget);
void                    ei_release_func                 (struct ei_widget_t*	widget);
#endif //PROJETC_IG_EI_PLACER_H
