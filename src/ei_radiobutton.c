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
ei_size_t min_size = {0, 0};

void                    text_size                               (ei_radio_button *to_draw,
                                                                         ei_rect_t *text_placer,
                                                                         ei_color_t *color,
                                                                         ei_surface_t *surface,
                                                                         ei_rect_t *clipper,
                                                                         char* text){
        char *etc = "...";
        int width_of_etc;
        int height_of_etc;
        int width_of_text;
        int height_of_text;
        hw_text_compute_size(etc, ei_default_font, &width_of_etc, &height_of_etc);
        hw_text_compute_size(text, ei_default_font, &width_of_text, &height_of_text);
        ei_rect_t text_rect = * text_placer;


        int left_space = text_placer->size.width;
        if (width_of_text > left_space){
                if (width_of_etc < left_space){

                        ei_rect_t etc_placer;

                        text_rect.size.width            -= width_of_etc;
                        etc_placer.top_left             = text_rect.top_left;
                        etc_placer.top_left.x           += text_rect.size.width;
                        etc_placer.size.width           = width_of_etc;
                        etc_placer.size.height          = height_of_etc;


                        draw_text(etc, ei_default_font, &etc_placer, ei_anc_northwest, surface, *color, clipper);
                        draw_text(text, ei_default_font, &text_rect, ei_anc_northwest, surface, *color, clipper);
                }

        }
        draw_text(text, ei_default_font, &text_rect, ei_anc_northwest, surface, *color, clipper);


}
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

        //setting the rect height to the header height
        ei_rect_t allow_rec = inter_rect(clipper, &widget->screen_location);
        ei_rect_t   text_rect = widget->screen_location;
        //dark title
        //drawing the background
        ei_fill(pick_surface, widget->pick_color, &allow_rec);
        ei_fill(surface, &to_draw->bg_color, &allow_rec);

        ei_rect_t dborder = widget->screen_location;
        dborder.top_left.y += to_draw->title_height / 2;
        dborder.size.height -= to_draw->title_height / 2;
        rectangle(&dborder, to_draw->tb_color, surface, BORDER_SIZE, MARGIN_LEFT, to_draw->title_width, clipper);

        text_rect.top_left.x += MARGIN_LEFT;
        text_rect = inter_rect(&widget->screen_location, &text_rect);
        draw_text(to_draw->title, ei_default_font, &text_rect, ei_anc_northwest, surface, to_draw->tb_color, clipper);
        text_rect.size.width -= 2 * to_draw->title_height;
        text_rect.top_left.x += to_draw->title_height + 2 * MARGIN_LEFT;
        for (int i = 0; i < to_draw->nb_of_choices; i++) {
                text_rect.top_left.y += 2 * to_draw->title_height;
                text_size(to_draw, &text_rect, &to_draw->tb_color, surface, &allow_rec, to_draw->choices[i]);
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
        free(widget->content_rect);
}

void*                    ei_radiobutton_allofunc                                (void){
        ei_radio_button *new_radiobutton = safe_malloc(sizeof(ei_radio_button));
        new_radiobutton->widget.content_rect = safe_malloc(sizeof(ei_rect_t));
        return new_radiobutton;
}

void                    ei_radiobutton_geomnotifyfunc                           (struct ei_widget_t* widget){
        ei_app_invalidate_rect(&(widget->screen_location));
}

void                    ei_radiobutton_setdefaults                              (ei_widget_t      *widget){

        ei_color_t red = {255, 0, 0, 255};
        ei_color_t black = {0, 0, 0, 255};
        ei_radio_button *to_configure = (ei_radio_button *)widget;
        to_configure->title                     = "Tadam";
        to_configure->current_button            = NULL;
        to_configure->choices                   = NULL;
        to_configure->requested_size            = ei_size(0, 0);
        to_configure->nb_of_choices             = 0;
        to_configure->b_rel_col                 = ei_default_background_color;
        to_configure->tb_color                  = black;
        to_configure->bg_color                  = ei_default_background_color;
        to_configure->b_press_col               = red;


}

ei_bool_t process_radio(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
        ei_radio_button *parent = (ei_radio_button *)widget->parent;

        if (parent->current_button != NULL){
                if (parent->current_button == widget)
                        return EI_TRUE;
                ei_button_configure(parent->current_button, NULL, &parent->b_rel_col,
                                    NULL, NULL, NULL, NULL, NULL,
                                    NULL, NULL, NULL, NULL, NULL,
                                    NULL, NULL);
        }
        parent->current_button = widget;
        ei_button_configure(widget, NULL, &parent->b_press_col,
                            NULL, NULL, NULL, NULL, NULL,
                            NULL, NULL, NULL, NULL, NULL,
                            NULL, NULL);
        printf("%s \n", data(widget->parent));
        return EI_TRUE;


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
                                                                                 int             *nb_of_choices,
                                                                                 ei_color_t      *text_border_color,
                                                                                 ei_color_t      *background_color,
                                                                                 ei_color_t      *press_col,
                                                                                 ei_color_t      *rel_color){
        ei_radio_button *to_configure = (ei_radio_button *)widget;
        is_defined(widget)              ? to_configure->widget          = *widget               : do_nothing();
        is_defined(requested_size)      ? to_configure->requested_size  = *requested_size       : do_nothing();
        is_defined(title)               ? to_configure->title           = *title                : do_nothing();
        is_defined(nb_of_choices)       ? to_configure->nb_of_choices   = *nb_of_choices        : do_nothing();
        is_defined(choices)             ? to_configure->choices         = *choices              : do_nothing();
        is_defined(text_border_color)   ? to_configure->tb_color        = *text_border_color    : do_nothing();
        is_defined(background_color)    ? to_configure->bg_color        = *background_color     : do_nothing();
        is_defined(press_col)           ? to_configure->b_press_col     = *press_col            : do_nothing();
        is_defined(rel_color)           ? to_configure->b_rel_col       = *rel_color            : do_nothing();


        int total_height = 0;
        hw_text_compute_size(to_configure->title, ei_default_font, &to_configure->title_width, &to_configure->title_height);
        ei_widget_t *current;
        ei_size_t size_button = ei_size(to_configure->title_height, to_configure->title_height);

        int left_side = 2 * MARGIN_LEFT;
        int radius = 10;
        ei_callback_t button_callback = process_radio;

        for (int i = 0; i < to_configure->nb_of_choices; i++) {
                total_height += 2 * to_configure->title_height;
                current = ei_widget_create("button", widget, NULL, NULL);
                ei_button_configure(current, &size_button, &to_configure->b_rel_col,
                        NULL, &radius, NULL, NULL, NULL,
                        NULL, NULL, NULL, NULL, NULL,
                        &button_callback, NULL);
                ei_place(current, NULL, &left_side, &total_height, NULL, NULL, NULL, NULL, NULL, NULL);
        }

        total_height += to_configure->title_height;
        if (!is_defined(requested_size)){
                to_configure->requested_size.width = to_configure->title_width + 2 * MARGIN_LEFT + to_configure->title_height;
                to_configure->requested_size.height = total_height + 2 * MARGIN_LEFT;
        }
        if (total_height > to_configure->requested_size.height)
                to_configure->requested_size.height = total_height;

        if (to_configure->requested_size.height < min_size.height )
                to_configure->requested_size.height = min_size.height;
        if (to_configure->requested_size.width < min_size.width )
                to_configure->requested_size.width = min_size.width;
        widget->requested_size = to_configure->requested_size;

        ei_app_invalidate_rect(&widget->screen_location);

}