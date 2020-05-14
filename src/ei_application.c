#include <stdlib.h>
#include <stdbool.h>


#include "ei_application.h"
#include "ei_event.h"
#include "ei_widgetclass.h"
#include "ei_widget.h"


ei_surface_t main_window;
ei_surface_t pick_surface;
ei_widget_t* rootWidget;
bool running = true;


void next_sibling_widget(ei_widget_t** widget_pt)
{
	*widget_pt = (*widget_pt)->next_sibling;
}


void destroy_widgets(ei_widget_t* begin)
{
	ei_widget_t* child = begin->children_head;
	for (child; child != NULL; next_sibling_widget(&child)) {
		destroy_widgets(child);
	}
	ei_widget_destroy(begin);
}


void draw_widgets      (ei_widget_t* begin,
			ei_surface_t surface,
			ei_surface_t pick_surface)
{
	ei_widget_t* child = begin->children_head;
	for (child; child != NULL; next_sibling_widget(&child)) {
		draw_widgets(child, surface, pick_surface);
	}
	if (begin->parent != NULL) {
		begin->wclass->drawfunc(begin, surface, pick_surface, &(begin->parent->screen_location));
	}
	else {
		begin->wclass->drawfunc(begin, surface, pick_surface, NULL);
	}

}





void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen)
{
	hw_init();
	main_window = hw_create_window(main_window_size, fullscreen);
	pick_surface = hw_surface_create(main_window, main_window_size, false);
	ei_frame_register_class();
	rootWidget = ei_widget_create("frame", NULL, NULL, NULL);
}


void ei_app_free(void)
{
	destroy_widgets(rootWidget);
	hw_quit();
}

void ei_app_quit_request(void)
{
	running = false;
}

void ei_app_run(void)
{
	ei_event_t event;
	event.type = ei_ev_none;
	while (running) {
		draw_widgets(rootWidget, main_window, pick_surface);
		if (event.type == ei_ev_keydown) {
			ei_app_quit_request();
		}
		hw_event_wait_next(&event);
	}
}


ei_widget_t* ei_app_root_widget(void)
{
	return  rootWidget;
}



