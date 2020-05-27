//
// Created by aymeric on 27/05/2020.
//

//
// Created by aymeric on 27/05/2020.
//
#include <ei_application.h>
#include "ei_radiobutton.h"
#include "utils.h"
#include "ei_widget.h"
#include "ei_types.h"
#include "ei_utils.h"
#include "draw_tools.h"
#include "event.h"

void                    ei_radiobutton_drawfunc                                  (struct	ei_widget_t*	widget,
                                                                                 ei_surface_t	surface,
                                                                                 ei_surface_t	pick_surface,
                                                                                 ei_rect_t*	clipper){
        hw_surface_lock(surface);
        ei_radio_button *to_draw = (ei_radio_button *)widget;
        //settings of the pick colors for the events
        if (widget->pick_color == NULL) {
                ei_color_t* pick_color = malloc(sizeof(ei_color_t));
                *pick_color = ei_map_color(pick_surface, widget->pick_id);
                widget->pick_color = pick_color;
        }
        int border_size = 2;
        //setting the rect height to the header height
        ei_rect_t allow_rec = inter_rect(clipper, &widget->screen_location);
        ei_rect_t   text_rect = allow_rec;
        //dark title
        ei_color_t dark = {0, 0, 0};

        allow_rec.top_left.y += to_draw->title_height/2;
        allow_rec = inter_rect(clipper, &allow_rec);
        ei_color_t red = {0, 0, 0, 255};
        ei_color_t white = {255, 255, 225, 255};
        ei_fill(surface, &white, &allow_rec);
        ei_fill(pick_surface, widget->pick_color, &allow_rec);
        rectangle(&allow_rec, red, surface, border_size, MARGIN_LEFT, to_draw->title_width);

        text_rect.top_left.x += MARGIN_LEFT;
        text_rect = inter_rect(&widget->screen_location, &text_rect);
        draw_text(to_draw->title, ei_default_font, &text_rect, ei_anc_northwest, surface, dark, clipper);

        text_rect.top_left.x += to_draw->title_height + MARGIN_LEFT;
        for (int i = 0; i < to_draw->nb_of_choices; ++i) {
                text_rect.top_left.y += 2 * to_draw->title_height;
                text_rect = inter_rect(&widget->screen_location, &text_rect);
                draw_text(to_draw->choices[i], ei_default_font, &text_rect, ei_anc_northwest, surface, dark, clipper);
        }

        hw_surface_unlock(surface);
}
char*                   data                                                    (ei_widget_t *widget){
        ei_radio_button  *radio = (ei_radio_button *)widget;
        if (widget->children_head){
                ei_widget_t *child = widget->children_head;
                for (int i = 0; i < radio->nb_of_choices; ++i) {
                        if (child == radio->current_button)
                                return radio->choices[i];
                        child = child->next_sibling;
                }
        }
        return NULL;

}
void                    ei_radiobutton_releasefunc                              (ei_widget_t*	widget){
        do_nothing();
}

void*                    ei_radiobutton_allofunc                                (void){
        ei_radio_button *new_radiobutton = safe_malloc(sizeof(ei_radio_button));
        return new_radiobutton;
}

void                    ei_radiobutton_geomnotifyfunc                           (struct ei_widget_t* widget){
        ei_app_invalidate_rect(&(widget->screen_location));
}

void                    ei_radiobutton_setdefaults                              (ei_widget_t      *widget){
        widget->content_rect = &widget->screen_location;
        ei_radio_button *to_configure = (ei_radio_button *)widget;
        to_configure->title                     = "a";
        to_configure->current_button            = NULL;
        to_configure->choices                   = NULL;
        to_configure->requested_size            = ei_size(0, 0);
        to_configure->nb_of_choices             = 0;

}

ei_bool_t process_radio(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
        ei_radio_button *parent = (ei_radio_button *)widget->parent;

        if (parent->current_button != NULL){
                ei_color_t base = ei_default_background_color;
                ei_button_configure(parent->current_button, NULL, &base,
                                    NULL, NULL, NULL, NULL, NULL,
                                    NULL, NULL, NULL, NULL, NULL,
                                    NULL, NULL);
        }
        parent->current_button = widget;
        ei_color_t red = {255, 0, 0 ,255};
        ei_button_configure(widget, NULL, &red,
                            NULL, NULL, NULL, NULL, NULL,
                            NULL, NULL, NULL, NULL, NULL,
                            NULL, NULL);
        printf("%s \n", data(widget->parent));


}

void                    ei_radiobutton_register_class                            (void){
        ei_widgetclass_t* radiobutton = safe_malloc(sizeof(ei_radio_button));
        strcpy(radiobutton->name, "radiobutton");
        radiobutton->allocfunc = &ei_radiobutton_allofunc;
        radiobutton->releasefunc = &ei_radiobutton_releasefunc;
        radiobutton->drawfunc = &ei_radiobutton_drawfunc;
        radiobutton->setdefaultsfunc = &ei_radiobutton_setdefaults;
        radiobutton->geomnotifyfunc = &ei_radiobutton_geomnotifyfunc;
        radiobutton->next = NULL;
        ei_widgetclass_register(radiobutton);
}

void                    ei_radiobutton_configure                                (ei_widget_t      *widget,
                                                                                 ei_size_t       *requested_size,
                                                                                 char            ***choices,
                                                                                 char            **title,
                                                                                 int             *nb_of_choices){
        ei_radio_button *to_configure = (ei_radio_button *)widget;
        is_defined(widget)              ? to_configure->widget          = *widget               : do_nothing();
        is_defined(requested_size)      ? to_configure->requested_size  = *requested_size       : do_nothing();
        is_defined(title)               ? to_configure->title           = *title                : do_nothing();
        is_defined(nb_of_choices)       ? to_configure->nb_of_choices   = *nb_of_choices        : do_nothing();
        is_defined(choices)             ? to_configure->choices         = *choices              : do_nothing();


        int total_height = 0;
        hw_text_compute_size(to_configure->title, ei_default_font, &to_configure->title_width, &to_configure->title_height);
        ei_widget_t *current;
        ei_size_t size_button = ei_size(to_configure->title_height, to_configure->title_height);

        int left_side = MARGIN_LEFT;
        int radius = 5;
        ei_callback_t button_callback = process_radio;

        for (int i = 0; i < to_configure->nb_of_choices; i++) {
                total_height += 2 * to_configure->title_height;
                current = ei_widget_create("button", widget, NULL, NULL);
                ei_button_configure(current, &size_button, NULL,
                        NULL, &radius, NULL, NULL, NULL,
                        NULL, NULL, NULL, NULL, NULL,
                        &button_callback, NULL);
                ei_place(current, NULL, &left_side, &total_height, NULL, NULL, NULL, NULL, NULL, NULL);
        }

        total_height += to_configure->title_height;
        if (!is_defined(requested_size)){
                to_configure->requested_size.width = to_configure->title_width + 2 * MARGIN_LEFT + to_configure->title_height;
                to_configure->requested_size.height = total_height;
        }
        if (total_height > to_configure->requested_size.height)
                to_configure->requested_size.height = total_height;

        if (to_configure->requested_size.width < to_configure->title_width + 2 * MARGIN_LEFT + to_configure->title_height)
                to_configure->requested_size.width = to_configure->title_width + 2 * MARGIN_LEFT + to_configure->title_height;
        widget->requested_size = to_configure->requested_size;
        ei_app_invalidate_rect(&widget->screen_location);

}