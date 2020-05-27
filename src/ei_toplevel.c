/******************************************************************************
* File:             ei_toplevel.c
*
* Author:           Robin Bertin, Aymeric Devriésère, Louise Lallemand
* Created:          05/26/20
* Description:      All functions and structure for toplevel type widget  
*****************************************************************************/

//
// Created by aymeric on 19/05/2020.
//
#include <stdlib.h>
#include <string.h>

#include "ei_toplevel.h"
#include "utils.h"
#include "ei_utils.h"
#include "ei_widgetclass.h"
#include "hw_interface.h"
#include "ei_application.h"
#include "ei_types.h"
#include "draw_tools.h"
#include "ei_event.h"
#include "callfunction.h"
#include "ei_placer.h"
ei_size_t       min_size_default        = {160, 120};
ei_size_t       default_rt_size         = {16, 16};
ei_widget_t     *first_child_opti;



void*                    ei_toplevel_allofunc                            (void){
        ei_toplevel *new_top_level = safe_malloc(sizeof(ei_toplevel));
        new_top_level->sub_frame = ei_widget_create("frame", (ei_widget_t *)new_top_level, NULL, NULL);
        new_top_level->widget.content_rect = safe_malloc(sizeof(ei_rect_t));
        return new_top_level;
}

void                    ei_toplevel_releasefunc                         (ei_widget_t*	widget){
        //nothing to do here, everything is released when the children are released
	if (widget->pick_color) {
		free(widget->pick_color);
	}
	free(widget->content_rect);
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

void                    compute_text_size                               (ei_toplevel *to_draw,
                                                                        ei_rect_t *text_placer,
                                                                        ei_color_t *color,
                                                                        ei_surface_t *surface,
                                                                        ei_rect_t *clipper){
        char *etc = " ...  ";
        int width_of_etc;
        int height_of_etc;
        hw_text_compute_size(etc, ei_default_font, &width_of_etc, &height_of_etc);
        text_placer->top_left.x += 2 * MARGIN_LEFT + to_draw->close_button->requested_size.width * 2;
        text_placer->top_left.y += MARGIN_TOP;

        int left_space = to_draw->widget.screen_location.size.width
                                - (2 * MARGIN_LEFT + to_draw->close_button->requested_size.width * 2);
        text_placer->size.width = left_space;
        if (to_draw->title_width > left_space){
                if (width_of_etc < left_space){

                        ei_rect_t etc_placer;

                        text_placer->size.width         -= width_of_etc;
                        etc_placer.top_left             = text_placer->top_left;
                        etc_placer.top_left.x           += text_placer->size.width;
                        etc_placer.size.width           = width_of_etc;
                        etc_placer.size.height          = height_of_etc;


                        draw_text(etc, ei_default_font, &etc_placer, ei_anc_northwest, surface, *color, clipper);
                }

        }

        text_placer->size.height = to_draw->title_height;

}

void                    ei_toplevel_drawfunc                            (struct	ei_widget_t*	widget,
                                                                                ei_surface_t	surface,
                                                                                ei_surface_t	pick_surface,
                                                                                ei_rect_t*	clipper){
        hw_surface_lock(surface);
        ei_toplevel *toplevel = (ei_toplevel *)widget;
	if (widget->pick_color == NULL) {
		ei_color_t* pick_color = malloc(sizeof(ei_color_t));
		*pick_color = ei_map_color(pick_surface, widget->pick_id);
		widget->pick_color = pick_color;
		if (toplevel->resizable != ei_axis_none){
                        ei_color_t* pick_color_rt = malloc(sizeof(ei_color_t));
                        *pick_color_rt = ei_map_color(pick_surface, widget->pick_id);
                        toplevel->resize_tool->pick_color = pick_color_rt;
		}


	}
        ei_toplevel *to_draw            = (ei_toplevel *)widget;
        ei_rect_t   rect_tot;
        ei_rect_t   allow_rec;
        rect_tot = widget->screen_location;
        //setting the rect height to the header height
        rect_tot.size.height = to_draw->title_height + 2 * MARGIN_TOP;
        allow_rec = inter_rect(clipper, &rect_tot);



        //rounded top corner drawing
        ei_linked_point_t* rounded0 = rounded_top_level(&rect_tot, 20, 0);
        ei_draw_polygon(surface, rounded0, to_draw->color, &allow_rec);
	ei_draw_polygon(pick_surface, rounded0, *(widget->pick_color), &allow_rec);
        free_linked_point_list(rounded0);

        //dark title
        ei_color_t dark = {0, 0, 0};
        ei_rect_t text_placer = rect_tot;

        compute_text_size(to_draw, &text_placer, &dark, surface, clipper);
        //position of text

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
        int y_subframe = MARGIN_TOP * 2 + to_configure->title_height;

        ei_size_t cb_size;

        //settings of the resize tools
        if (to_configure->resizable != ei_axis_none){
                ei_frame_configure(to_configure->resize_tool, &default_rt_size, &rt_color, 0,
                                   NULL, NULL, NULL, NULL, NULL,
                                   NULL, NULL,  NULL);
                ei_place(to_configure->resize_tool, &anchor_rt, NULL, NULL, NULL, NULL, &rel_x_rt, &rel_y_rt, NULL, NULL);

        }
        //settings of the subframe
        float rel_x = 0;
        float rel_y = 0;
        float rel_width = 1;
        float rel_height = 1;
        ei_frame_configure(to_configure->sub_frame, NULL, &to_configure->color, &to_configure->border_width,
                           NULL, NULL, NULL, NULL, NULL,
                           NULL, NULL,  NULL);
//        to_configure->widget.content_rect       = to_configure->sub_frame->content_rect;
        ei_place(to_configure->sub_frame, NULL, NULL, NULL, NULL
                , NULL, &rel_x, &rel_y, &rel_width, &rel_height);

        //settings of the close_button
        if (to_configure->closable == EI_TRUE){

                cb_size.height = to_configure->title_height/1.5;
                cb_size.width = to_configure->title_height/1.5;

                int             middle_text_pos         = to_configure->title_height/2 + MARGIN_TOP;
                int             middle_button_pos       = cb_size.height + MARGIN_LEFT;
                int             radius                  = 6;
                int             border_test             = 0;
                ei_anchor_t     bc_anchor               = ei_anc_center;

		ei_callback_t button_close_top = close_toplevel;
                ei_button_configure(to_configure->close_button, &cb_size, &cb_color, &border_test, &radius, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &button_close_top, NULL);
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

        int marging_height = MARGIN_TOP * 2 + to_configure->title_height;

        widget->requested_size.height   = max(to_configure->min_size->height, to_configure->requested_size.height);
        widget->requested_size.width    = max(to_configure->min_size->width, to_configure->requested_size.width);

        //adding 2 times boder width for the size of the subframe
        widget->requested_size.height   += to_configure->border_width * 2;
        widget->requested_size.width    += to_configure->border_width * 2;

        widget->content_rect->top_left.x = to_configure->widget.screen_location.top_left.x ;
        widget->content_rect->top_left.y = to_configure->widget.screen_location.top_left.y + MARGIN_TOP * 2 + to_configure->title_height;
        widget->content_rect->size.width = widget->screen_location.size.width;
        widget->content_rect->size.height = widget->screen_location.size.height;

                //configuration of closing button, resize_tool and subframe
        configure_sub_part(to_configure);


        //the content rect is now the subframe (all the widget exept the header)
//        widget->content_rect = to_configure->sub_frame->content_rect;

        widget->requested_size.height   += marging_height;

        if (border_width != NULL){
                ei_place(widget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        }
}


ei_bool_t close_toplevel(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	ei_widget_t* parent = widget->parent;
	if (strcmp(parent->wclass->name, "toplevel") == 0) {
		ei_widget_destroy(parent);
		return EI_TRUE;
	}
	return EI_FALSE;
}


ei_bool_t dispatch_event(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	int x_mouse = event->param.mouse.where.x;
	int y_mouse = event->param.mouse.where.y;
	int x_widget = widget->screen_location.top_left.x;
	int y_widget = widget->screen_location.top_left.y;
	int h_widget = widget->screen_location.size.height;
	int w_widget = widget->screen_location.size.width;
	if (y_mouse > y_widget && y_mouse < y_widget + 25) {
                return move_top_down(widget, event, user_param);
        }
        if (x_mouse > x_widget + w_widget - default_rt_size.width && y_mouse > y_widget + h_widget - default_rt_size.width) {
		return resize_top_down(widget, event, user_param);
	}
	return EI_FALSE;
}

ei_bool_t resize_top_down(ei_widget_t* widget, ei_event_t* event, void* user_param){
        ei_point_t* oldPoint = malloc(sizeof(ei_point_t));
        oldPoint->x = event->param.mouse.where.x;
        oldPoint->y = event->param.mouse.where.y;
        ei_bind(ei_ev_mouse_move, NULL, "all", resize_top_onmove, (void *)oldPoint);
        ei_bind(ei_ev_mouse_buttonup, NULL, "all", resize_top_up, NULL);
        return EI_TRUE;
}

ei_bool_t resize_top_onmove(ei_widget_t* widget, ei_event_t* event, void* user_param){
        if (widget->wclass == ei_widgetclass_from_name("toplevel")){

                ei_toplevel *toplvel = (ei_toplevel *)widget;
                ei_point_t* oldPoint = (ei_point_t *)user_param;
                int x_mouse = event->param.mouse.where.x;
                int y_mouse = event->param.mouse.where.y;
                int ecart_width;
                int ecart_height;
                if (toplvel->resizable == ei_axis_x){
                        ecart_width = x_mouse - oldPoint->x + widget->screen_location.size.width;
                        ecart_height = widget->screen_location.size.height;
                }
                else if (toplvel->resizable == ei_axis_y){
                        ecart_height = y_mouse - oldPoint->y + widget->screen_location.size.height;
                        ecart_width = widget->screen_location.size.width;

                } else {
                        ecart_height = y_mouse - oldPoint->y + widget->screen_location.size.height;
                        ecart_width = x_mouse - oldPoint->x + widget->screen_location.size.width;
                }

                if (ecart_height >= toplvel->min_size->height && ecart_width >= toplvel->min_size->width)
                        ei_place(widget, NULL, NULL, NULL, &ecart_width, &ecart_height, NULL, NULL, NULL, NULL);

                oldPoint->x = x_mouse;
                oldPoint->y = y_mouse;
                return EI_TRUE;
        }

}

ei_bool_t resize_top_up(ei_widget_t* widget, ei_event_t* event, void* user_param){
        ei_unbind(ei_ev_mouse_move, NULL, "all", resize_top_onmove, user_param);
        ei_unbind(ei_ev_mouse_buttonup, NULL, "all", resize_top_up, user_param);
        free(user_param);
        return EI_TRUE;
}
ei_bool_t move_top_down(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	ei_point_t* oldPoint = malloc(sizeof(ei_point_t));
	oldPoint->x = event->param.mouse.where.x;
        oldPoint->y = event->param.mouse.where.y;
//        tampon = widget->children_head;
//        widget->children_head = NULL;
        if (widget->wclass == ei_widgetclass_from_name("toplevel") && LOW_PROC){

                first_child_opti = widget->children_head;
                widget->children_head = NULL;
        }
	ei_bind(ei_ev_mouse_move, widget, NULL, move_top_onmove, (void *)oldPoint);
	ei_bind(ei_ev_mouse_buttonup, widget, NULL, move_top_up, NULL);
	return EI_TRUE;

}
ei_bool_t move_top_onmove(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
        if (widget->wclass == ei_widgetclass_from_name("toplevel")){
                ei_point_t* oldPoint = (ei_point_t *)user_param;
                ei_toplevel *toplevel = (ei_toplevel *)widget;
                ei_placer_t *w_placer = (ei_placer_t *)widget->geom_params;
                int x_mouse = event->param.mouse.where.x;
                int y_mouse = event->param.mouse.where.y;
                int ecart_x = x_mouse - oldPoint->x + w_placer->x;
                int ecart_y = y_mouse - oldPoint->y + w_placer->y;
                ei_place(widget, NULL, &ecart_x, &ecart_y, NULL, NULL, NULL, NULL, NULL, NULL);

                oldPoint->x = x_mouse;
                oldPoint->y = y_mouse;
                return EI_TRUE;
        }

}
void force_run(ei_widget_t* widget){
        ei_widget_t *child = widget->children_head;
        widget->wclass->geomnotifyfunc(widget);
        while(child){
                if (is_defined(child->geom_params))
                        ei_run_func(child);
                child = child->next_sibling;
        }
}


ei_bool_t move_top_up(ei_widget_t* widget, ei_event_t* event, void* user_param)
{

        if (widget->wclass == ei_widgetclass_from_name("toplevel") && LOW_PROC){

                widget->children_head = first_child_opti;
                force_run(widget);
        }
        ei_unbind(ei_ev_mouse_move, widget, NULL, move_top_onmove, user_param);
	ei_unbind(ei_ev_mouse_buttonup, widget, NULL, move_top_up, user_param);
	free(user_param);
	return EI_TRUE;
}
