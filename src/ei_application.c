#include "ei_application.h"
#include "ei_event.h"

void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen)
{
	hw_init();
	ei_surface_t main_window = hw_create_window(main_window_size, fullscreen);
}


void ei_app_free(void)
{
	hw_quit();
}


void ei_app_run(void)
{
	ei_event_t event;
	event.type = ei_ev_none;
	while (event.type != ei_ev_keydown) {
		hw_event_wait_next(&event);
	}
}


ei_widget_t* ei_app_root_widget(void)
{

}
