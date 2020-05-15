#include <stdlib.h>
#include <stdbool.h>


#include "ei_application.h"
#include "ei_event.h"
#include "ei_widgetclass.h"
#include "ei_widget.h"
#include "traverse_tools.h"


ei_surface_t main_window;
ei_surface_t pick_surface;
ei_widget_t* rootWidget;
bool running = true;


void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen)
{
	hw_init();
	main_window = hw_create_window(main_window_size, fullscreen);
	pick_surface = hw_surface_create(main_window, main_window_size, false);
	ei_frame_register_class();
	ei_button_register_class();
	rootWidget = ei_widget_create("frame", NULL, NULL, NULL);
	ei_frame_configure(rootWidget, &main_window_size, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}


void ei_app_free(void)
{
	hw_surface_free(pick_surface);
	hw_surface_free(main_window);
	ei_widget_destroy(rootWidget);
	ei_widgetclass_t* sentinel = ei_widgetclass_from_name("sentinel");
	if (sentinel->next != NULL) {
		destroy_widgetclass(sentinel->next);
	}
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
