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
ei_size_t       min_size_default = {160, 120};
ei_size_t       default_tool_size = {16, 16};
int             margin_left     = 5;
int             margin_top      = 0;

void*                    ei_toplevel_allofunc                            (void){
        ei_toplevel *new_top_level = safe_malloc(sizeof(ei_toplevel));
        new_top_level->sub_frame = ei_widget_create("frame", new_top_level, NULL, NULL);
        new_top_level->resize_tool = ei_widget_create("frame", new_top_level->sub_frame, NULL, NULL);
        return new_top_level;
}

void                    ei_toplevel_releasefunc                         (ei_widget_t*	widget){

}

void                    ei_toplevel_setdefaultsfunc                     (ei_widget_t* widget){
        ei_toplevel *to_configure               = (ei_toplevel *)widget;

        ei_color_t color                = {255, 0, 0, 0};
        to_configure->color             = ei_default_background_color;
        to_configure->border_width      = 4;
        to_configure->requested_size    = ei_size(320, 240);
        to_configure->title             = "Toplevel";
        to_configure->closable          = EI_TRUE;
        to_configure->resizable         = ei_axis_both;
        to_configure->min_size          = NULL;
        to_configure->title_width       = 0;
        to_configure->title_height      = 0;



//        hw_text_compute_size(to_configure->title, ei_default_font, &taille_texte.width, &taille_texte.width);
//        to_configure->requested_size.height += taille_texte.height;






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
        rect_to_fill->top_left.x        = rect_tot.top_left.x;
        rect_to_fill->top_left.y        = rect_tot.top_left.y;
        rect_to_fill->size.width        = rect_tot.size.width;
        rect_to_fill->size.height       = rect_tot.size.height;
//        rect_to_fill->size.width        = rect_tot.size.width           - 2 * to_draw->border_width;
//        rect_to_fill->size.height       = rect_tot.size.height          - 2 * to_draw->border_width;

        ei_point_t  * text_pos;

        if (is_defined(to_draw->title)){

                int text_width;
                int text_height;
                hw_text_compute_size(to_draw->title, ei_default_font, &text_width, &text_height);
                text_pos = anchor_point(rect_to_fill, ei_anc_northwest, text_width, text_width);
        }

        ei_rect_t rectangle_to_fill = inter_rect(clipper, rect_to_fill);
        ei_color_t header_color = {255, 25, 25};
        ei_fill(surface, &to_draw->color, &rectangle_to_fill);
        ei_point_t* point_text;
        int height_text;
        int width_text;
        ei_font_t font  = ei_default_font;
        ei_color_t dark = {0, 0, 0};
        ei_rect_t text_placer = *rect_to_fill;
        text_placer.top_left.x += 10;
        text_placer.top_left.y += 5;
        point_text = anchor_point(&text_placer, ei_anc_northwest, width_text,height_text);
        ei_draw_text(surface, point_text, to_draw->title, font, dark, &text_placer);
        free(point_text);



        hw_surface_unlock(surface);

        //on libère la mémoire
        free(rect_to_fill);


}


void			ei_toplevel_register_class 	          (void){
        ei_widgetclass_t* toplevel = safe_malloc(sizeof(ei_widgetclass_t));
        strcpy(toplevel->name, "toplevel");
        toplevel->allocfunc = &ei_toplevel_allofunc;
        toplevel->releasefunc = &ei_toplevel_releasefunc;
        toplevel->drawfunc = &ei_toplevel_drawfunc;
        toplevel->setdefaultsfunc = &ei_toplevel_setdefaultsfunc;
        toplevel->geomnotifyfunc = &ei_toplevel_geomnotifyfunc;
        toplevel->next = NULL;
        ei_widgetclass_register(toplevel);

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

        is_defined(widget)              ? to_configure->widget          = *widget               : do_nothing();
        is_defined(requested_size)      ? to_configure->requested_size  = *requested_size       : do_nothing();
        is_defined(color)               ? to_configure->color           = *color                : do_nothing();
        is_defined(closable)            ? to_configure->closable        = *closable             : do_nothing();
        is_defined(resizable)           ? to_configure->resizable       = *resizable            : do_nothing();
        is_defined(border_width)        ? to_configure->border_width    = *border_width         : do_nothing();
        // initialisation de la taille minimale
        if (is_defined(min_size)){
                to_configure->min_size = *min_size;
        } else {
                to_configure->min_size = &min_size_default;
        }
        hw_text_compute_size(to_configure->title, ei_default_font, &to_configure->title_width, &to_configure->title_height);

        is_defined(title)               ? to_configure->title           = *title                 : do_nothing();



        int marging_height = margin_top * 2 + to_configure->border_width * 2 + to_configure->title_height;
        int marging_width  = to_configure->border_width * 2;

        widget->requested_size.height   = max(to_configure->min_size->height, to_configure->requested_size.height);
        widget->requested_size.width    = max(to_configure->min_size->width, to_configure->requested_size.width);


        widget->requested_size.height   += to_configure->border_width * 2;
        widget->requested_size.width    += to_configure->border_width * 2;
        //content rect for children, starts just after window header
        int y_subframe = margin_top * 2 + to_configure->title_height + 2 * to_configure->border_width;
        ei_color_t sub_color = dark_color(to_configure->color);
        ei_frame_configure(to_configure->resize_tool, &default_tool_size, &sub_color, 0,
                           NULL, NULL, NULL, NULL, NULL,
                           NULL, NULL,  NULL);
        float rel_x = 1;
        float rel_y= 1;
        ei_anchor_t anchor = ei_anc_southeast;
        ei_color_t bluuue = {0, 0 ,255};
        ei_place(to_configure->resize_tool, &anchor, NULL, NULL, NULL, NULL, &rel_x, &rel_y, NULL, NULL);
        to_configure->sub_frame->content_rect = &widget->screen_location;
        widget->content_rect = to_configure->sub_frame->content_rect;
        ei_frame_configure(to_configure->sub_frame, &widget->requested_size, &to_configure->color, &to_configure->border_width,
                           NULL, NULL, NULL, NULL, NULL,
                           NULL, NULL,  NULL);
        ei_place(to_configure->sub_frame, NULL, NULL, &y_subframe, NULL, NULL, NULL, NULL, NULL, NULL);



        widget->requested_size.height   += marging_height;
        ei_app_invalidate_rect(&(widget->screen_location));


}
