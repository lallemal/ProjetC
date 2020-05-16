#include <stdlib.h>
#include <stdbool.h>


#include "ei_application.h"
#include "ei_event.h"
#include "ei_widgetclass.h"
#include "ei_widget.h"
#include "traverse_tools.h"

// Variables & definitions for linked list of rects
#define LIST_RECT_NORMAL 0
#define LIST_RECT_NULL 1
int rect_status;
ei_linked_rect_t* list_rect_head;
ei_linked_rect_t* list_rect_tail;

// Variables for the running of API
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


void ei_app_invalidate_rect(ei_rect_t* rect)
{
	if (rect_status != LIST_RECT_NULL) {
		if (rect == NULL) {
			free_linked_rect(list_rect_head);
			rect_status = LIST_RECT_NULL;
			list_rect_head = NULL;
			list_rect_tail = NULL;
		}
		else {
			ei_linked_rect_t* newElement = malloc(sizeof(ei_linked_rect_t));
			newElement->rect = *rect;
			newElement->next = NULL;
			if (list_rect_head == list_rect_tail) {
				list_rect_head = newElement;
				list_rect_tail = newElement;
			}
			list_rect_tail->next = newElement;
			list_rect_tail = newElement;
		}
	}
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
		rect_status = LIST_RECT_NORMAL;
		free_linked_rect(list_rect_head);
		draw_widgets(rootWidget, main_window, pick_surface);
		hw_surface_update_rects(main_window, list_rect_head);
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
