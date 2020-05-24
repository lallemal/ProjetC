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
ei_size_t       min_size_default        = {160, 120};
ei_size_t       default_rt_size         = {16, 16};
ei_size_t       default_cb_size         = {16, 16};
int             margin_top              = 5;
int             margin_left             = 10;

void*                    ei_toplevel_allofunc                            (void){
        ei_toplevel *new_top_level = safe_malloc(sizeof(ei_toplevel));
        new_top_level->sub_frame = ei_widget_create("frame", new_top_level, NULL, NULL);
        return new_top_level;
}

void                    ei_toplevel_releasefunc                         (ei_widget_t*	widget){
        //nothing to do here, everything is released when the children are released
}

void                    ei_toplevel_setdefaultsfunc                     (ei_widget_t* widget){
        ei_toplevel *to_configure               = (ei_toplevel *)widget;

        to_configure->color             = ei_default_background_color;
        to_configure->border_width      = 4;
        to_configure->requested_size    = ei_size(320, 240);
        to_configure->title             = "Toplevel";
        to_configure->closable          = EI_TRUE;
        to_configure->resizable         = ei_axis_both;
        to_configure->min_size          = NULL;
        to_configure->title_width       = 0;
        to_configure->title_height      = 0;
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
        ei_rect_t   rect_tot;

        rect_tot = widget->screen_location;
        //setting the rect height to the header height
        rect_tot.size.height = to_draw->title_height + 2 * margin_top;

        //rounded top corner drawing
        ei_linked_point_t* rounded0 = rounded_top_level(&rect_tot, 20, 0);
        ei_draw_polygon(surface, rounded0, to_draw->color, clipper);
        free_linked_point_list(rounded0);

        //dark title
        ei_color_t dark = {0, 0, 0};
        ei_rect_t text_placer = rect_tot;
        //position of text
        text_placer.top_left.x = to_draw->close_button->screen_location.top_left.x + to_draw->close_button->screen_location.size.width * 2;
        text_placer.top_left.y += margin_top;
        text_placer.size.width = to_draw->title_width;
        text_placer.size.height = to_draw->title_height;


        draw_text(to_draw->title, ei_default_font, &text_placer, ei_anc_northwest, surface, dark, clipper);

        hw_surface_unlock(surface);



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

void                    configure_sub_part                           (ei_toplevel *to_configure){
        //settings x and y rel for resize tool
        float rel_x_rt = 1;
        float rel_y_rt = 1;

        //setting the ancher of resize tool
        ei_anchor_t anchor_rt = ei_anc_southeast;

        //setting red for the closing button
        ei_color_t cb_color = {255, 0,0, 255};

        //darker color for the resize tool
        ei_color_t rt_color = dark_color(to_configure->color);

        // setting the y axis of subframe in order to be under the header
        int y_subframe = margin_top * 2 + to_configure->title_height;

        ei_size_t cb_size;

        //settings of the resize tools
        if (to_configure->resizable != ei_axis_none){
                ei_frame_configure(to_configure->resize_tool, &default_rt_size, &rt_color, 0,
                                   NULL, NULL, NULL, NULL, NULL,
                                   NULL, NULL,  NULL);
                ei_place(to_configure->resize_tool, &anchor_rt, NULL, NULL, NULL, NULL, &rel_x_rt, &rel_y_rt, NULL, NULL);

        }
        
        //settings of the subframe
        ei_frame_configure(to_configure->sub_frame, &to_configure->widget.requested_size, &to_configure->color, &to_configure->border_width,
                           NULL, NULL, NULL, NULL, NULL,
                           NULL, NULL,  NULL);
        to_configure->widget.content_rect       = to_configure->sub_frame->content_rect;
        ei_place(to_configure->sub_frame, NULL, NULL, &y_subframe, NULL, NULL, NULL, NULL, NULL, NULL);

        //settings of the close_button
        if (to_configure->closable == EI_TRUE){

                cb_size.height = to_configure->title_height/1.5;
                cb_size.width = to_configure->title_height/1.5;

                int             middle_text_pos         = to_configure->title_height/2 + margin_top;
                int             middle_button_pos       = cb_size.height + margin_left;
                int             radius                  = 6;
                int             border_test             = 0;
                ei_anchor_t     bc_anchor               = ei_anc_center;

                ei_button_configure(to_configure->close_button, &cb_size, &cb_color, &border_test, &radius, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                ei_place(to_configure->close_button, &bc_anchor, &middle_button_pos, &middle_text_pos, NULL, NULL, NULL, NULL, NULL, NULL);

        }

}

void                    init_the_sub_tools                       (ei_toplevel *toplevel){
        //malloc of the subframe
        if (toplevel->resizable != ei_axis_none && toplevel->resize_tool == NULL)
                toplevel->resize_tool = ei_widget_create("frame", toplevel->sub_frame, NULL, NULL);
        //malloc of the closing button
        if (toplevel->closable == EI_TRUE && toplevel->close_button == NULL)
                toplevel->close_button = ei_widget_create("button", &toplevel->widget, NULL, NULL);
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



        init_the_sub_tools(to_configure);

        // minimum size initialisation
        if (is_defined(min_size)){
                to_configure->min_size = *min_size;
        } else {
                to_configure->min_size = &min_size_default;
        }

        is_defined(title)               ? to_configure->title           = *title                 : do_nothing();
        //if the requested size is lower than the min size, then the size is set to the min size
        if (requested_size->width < to_configure->min_size->width) {
                to_configure->requested_size.width = to_configure->min_size->width;
        }

        //calculating the needed space for title
        hw_text_compute_size(to_configure->title, ei_default_font, &to_configure->title_width, &to_configure->title_height);

        if (requested_size->height < to_configure->min_size->height) {
                to_configure->requested_size.height = to_configure->min_size->height;
        }

        int marging_height = margin_top * 2 + to_configure->title_height;

        widget->requested_size.height   = max(to_configure->min_size->height, to_configure->requested_size.height);
        widget->requested_size.width    = max(to_configure->min_size->width, to_configure->requested_size.width);

        //adding 2 times boder width for the size of the subframe
        widget->requested_size.height   += to_configure->border_width * 2;
        widget->requested_size.width    += to_configure->border_width * 2;

        //configuration of closing button, resize_tool and subframe
        configure_sub_part(to_configure);


        //the content rect is now the subframe (all the widget exept the header)
        widget->content_rect = to_configure->sub_frame->content_rect;

        widget->requested_size.height   += marging_height;


}
