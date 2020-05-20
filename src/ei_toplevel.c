//
// Created by aymeric on 19/05/2020.
//
#include "ei_toplevel.h"
#include "utils.h"
#include "ei_utils.h"
#include "ei_widgetclass.h"
#include "hw_interface.h"
#include "ei_application.h"
#include "ei_types.h"
#include "draw_tools.h"


void*                    ei_toplevel_allofunc                            (void){
        ei_toplevel *new_top_level = safe_malloc(sizeof(ei_toplevel));
        return new_top_level;
}

void                    ei_toplevel_releasefunc                         (ei_widget_t*	widget){
        do_nothing();
}

void                    ei_toplevel_setdefaultsfunc                     (ei_widget_t* widget){
        ei_toplevel *to_configure               = (ei_toplevel *)widget;

        to_configure->requested_size    = ei_size(320, 240);
        to_configure->color             = ei_default_background_color;
        to_configure->border_width      = 4;
        to_configure->title             = "Toplevel";
        to_configure->closable          = EI_TRUE;
        to_configure->resizable         = ei_axis_both;
        to_configure->min_size = NULL;
//        ei_size_t* min_size = malloc(sizeof(ei_size_t));
//        *min_size = ei_size(160, 120);
        // changer le content rect pour que ca commende en dessous de l'entete


}


void                    ei_toplevel_geomnotifyfunc                      (struct ei_widget_t* widget){
        ei_app_invalidate_rect(&(widget->screen_location));
}


void                    ei_toplevel_drawfunc                            (struct	ei_widget_t*	widget,
                                                                                ei_surface_t	surface,
                                                                                ei_surface_t	pick_surface,
                                                                                ei_rect_t*	clipper){
        hw_surface_lock(surface);
        ei_toplevel *to_draw            = (ei_toplevel *)widget;
        ei_rect_t   *rect_to_fill       = safe_malloc(sizeof(ei_rect_t));
        ei_rect_t   rect_tot;


        if (!is_defined(clipper)){
                rect_tot = hw_surface_get_rect(surface);
        } else {
                rect_tot = inter_rect(clipper, &widget->screen_location);;
        }
//        rect_to_fill->top_left.x        = rect_tot.top_left.x;
//        rect_to_fill->top_left.y        = rect_tot.top_left.y           + to_draw->en_tete.height;
//        rect_to_fill->size.width        = rect_tot.size.width           - 2*(to_draw->border);
//        rect_to_fill->size.height       = rect_tot.size.height          - 2*(to_draw->border);
//
//        ei_point_t  * text_pos;
//
//        if (is_defined(to_draw->title)){
//
//                int text_width;
//                int text_height;
//                hw_text_compute_size(to_draw->title, to_draw->title_font, &text_width, &text_height);
//                text_pos = anchor_point(rect_to_fill, ei_anc_northwest, text_width, text_width);
//        }

        ei_rect_t rectangle_to_fill = inter_rect(clipper, rect_to_fill);

        ei_fill(surface, &to_draw->color, &rectangle_to_fill);

        hw_surface_unlock(surface);

        //on libère la mémoire
        free(rect_to_fill);


}


void			ei_toplevel_register_class 	          (void){
        ei_widgetclass_t* frame = malloc(sizeof(ei_widgetclass_t));
        strcpy(frame->name, "toplevel");
        frame->allocfunc = &ei_toplevel_allofunc;
        frame->releasefunc = &ei_toplevel_releasefunc;
        frame->drawfunc = &ei_toplevel_drawfunc;
        frame->setdefaultsfunc = &ei_toplevel_setdefaultsfunc;
        frame->geomnotifyfunc = &ei_toplevel_geomnotifyfunc;
        frame->next = NULL;
        ei_widgetclass_register(frame);

}

void			ei_toplevel_configure		          (ei_widget_t*		widget,
                                                                  ei_size_t*		requested_size,
                                                                  ei_color_t*		color,
                                                                  int*			border_width,
                                                                  char**	        title,
                                                                  ei_bool_t*		closable,
                                                                  ei_axis_set_t*	resizable,
                                                                  ei_size_t**		min_size){

        ei_toplevel* to_configure = (ei_toplevel *)widget;
        is_defined(widget)              ? to_configure->widget          = widget                : do_nothing();
        is_defined(requested_size)      ? to_configure->requested_size  = *requested_size       : do_nothing();
        is_defined(color)               ? to_configure->color           = *color                : do_nothing();
        is_defined(closable)            ? to_configure->closable        = *closable             : do_nothing();
        is_defined(resizable)           ? to_configure->resizable       = *resizable            : do_nothing();
        is_defined(min_size)            ? to_configure->min_size        = *min_size             : do_nothing();

        if (is_defined(title)){
                int height_text;
                int width_text;
                to_configure->title = title;
                hw_text_compute_size(*title, &ei_font_default_color, &width_text, &height_text);
                height_text += 2 * *border_width;
                width_text  += 2 * *border_width;
                widget->requested_size.height = max(widget->requested_size.height, height_text);
                widget->requested_size.width = max(widget->requested_size.width, width_text);


        }

        if (border_width != NULL){
                to_configure->border_width = *border_width;
//                widget->requested_size
        }




}
