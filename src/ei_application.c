#include <stdlib.h>
#include <stdbool.h>


#include "ei_application.h"
#include "ei_event.h"
#include "ei_widgetclass.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "traverse_tools.h"
#include "ei_types.h"

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
	rootWidget->screen_location.size.width = main_window_size.width;
	rootWidget->screen_location.size.height = main_window_size.height;
	ei_default_font = hw_text_font_create(ei_default_font_filename, ei_style_normal, ei_font_default_size);
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
			if (list_rect_head == NULL) {
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
	//hw_text_font_free(ei_default_font);
	hw_surface_free(pick_surface);
	hw_surface_free(main_window);
	ei_widget_destroy(rootWidget);

	ei_widgetclass_t* sentinel = ei_widgetclass_from_name("sentinel");
	if (sentinel->next != NULL) {
		destroy_widgetclass(sentinel->next);
	}

        ei_geometrymanager_t *current = ei_geometrymanager_from_name("sentinel");
	ei_geometrymanager_t *next    = current->next;
	while (next){
                current = next;
                next    = current->next;
                free(current);
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
		hw_surface_update_rects(main_window, list_rect_head);
		free_linked_rect(list_rect_head);
		list_rect_head = NULL;
		list_rect_tail = NULL;
		rect_status = LIST_RECT_NORMAL;
		if (event.type == ei_ev_keydown && event.param.key.key_code == 27) {
			ei_app_quit_request();
		}
		hw_event_wait_next(&event);
	}
}


ei_widget_t* ei_app_root_widget(void)
{
	return  rootWidget;
}
