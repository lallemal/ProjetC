#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


#include "ei_application.h"
#include "ei_event.h"
#include "ei_widgetclass.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "traverse_tools.h"
#include "ei_types.h"
#include "utils.h"

#include "callfunction.h"
#include "event.h"

// Variables & definitions for linked list of rects
#define LIST_RECT_PENDING 0
#define LIST_RECT_NULL 1
#define LIST_RECT_QUITTING 2
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
	// Initialize hardware and different surfaces.
	hw_init();
	main_window = hw_create_window(main_window_size, fullscreen);
	pick_surface = hw_surface_create(main_window, main_window_size, EI_FALSE);
	// Initialize core variables of the API : classes, placer, eventlist
	create_base_eventlist();
	ei_register_placer_manager();
	ei_frame_register_class();
	ei_toplevel_register_class();
	ei_button_register_class();

	// Binding of internal comportments
	ei_bind(ei_ev_mouse_buttondown, NULL, "button", button_on_press, NULL);
	// Creation of the root Widget
	rootWidget = ei_widget_create("frame", NULL, NULL, NULL);
	rootWidget->screen_location.size.width = main_window_size.width;
	rootWidget->screen_location.size.height = main_window_size.height;
	ei_frame_configure(rootWidget, &main_window_size, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	// Creation of the default font
	ei_default_font = hw_text_font_create(ei_default_font_filename, ei_style_normal, ei_font_default_size);
}


void ei_app_invalidate_rect(ei_rect_t* rect)
{
	if (rect_status != LIST_RECT_NULL && rect_status != LIST_RECT_QUITTING) {
		ei_linked_rect_t* newElement = malloc(sizeof(ei_linked_rect_t));
		ei_rect_t surface_rect = hw_surface_get_rect(main_window);
		if (newElement == NULL) {
			fprintf(stderr, "Out of Memory, Malloc failed \n");
			return;
		}
		if (rect == NULL || is_equal(rect, &surface_rect)) {
			ei_rect_t* copy = copy_rect(&surface_rect);
			newElement->rect = *copy;
			free(copy);
			newElement->next = NULL;
			free_linked_rect(list_rect_head);
			rect_status = LIST_RECT_NULL;
			list_rect_head = newElement;
			list_rect_tail = newElement;
		}
		else {
			ei_rect_t* copy = copy_rect(rect);
			newElement->rect = *copy;
			free(copy);
			// truncate the rect to the positive values.
			if (newElement->rect.top_left.x < 0) {
				newElement->rect.size.width += newElement->rect.top_left.x;
				newElement->rect.top_left.x = 0;
			}
			if (newElement->rect.top_left.y < 0) {
				newElement->rect.size.height += newElement->rect.top_left.y;
				newElement->rect.top_left.y = 0;
			}
			newElement->next = NULL;
			if (newElement->rect.size.height == 0 && newElement->rect.size.width == 0) {
				free(newElement);
				return;
			}
			if (list_rect_head == NULL) {
				list_rect_head = newElement;
				list_rect_tail = newElement;
			}
			else {
				list_rect_tail->next = newElement;
				list_rect_tail = newElement;
			}
			simplify_list(&list_rect_head);
		}
	}
}


void ei_app_free(void)
{
	//hw_text_font_free(ei_default_font);
	// Free the surfaces
	hw_surface_free(pick_surface);
	hw_surface_free(main_window);
	// Destroy rootWidget
	ei_widget_destroy(rootWidget);
	// Unbind internal comportments
	ei_unbind(ei_ev_mouse_buttondown, NULL, "button", button_on_press, NULL);
	// Destruction of core variables
	destroy_base_eventlist();

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

	// Hardware quitting
	hw_quit();
}



void ei_app_quit_request(void)
{
	running = false;
	rect_status = LIST_RECT_QUITTING;
}

void draw(void)
{
	ei_linked_rect_t* rectList = list_rect_head;
	while (rectList != NULL) {
		draw_widgets(rootWidget, main_window, pick_surface, rectList->rect);
		rectList = rectList->next;
	}
}


void ei_app_run(void)
{
	ei_event_t event;
	event.type = ei_ev_none;
	while (running) {
		draw();
		hw_surface_update_rects(main_window, list_rect_head);
		free_linked_rect(list_rect_head);
		list_rect_head = NULL;
		list_rect_tail = NULL;
		rect_status = LIST_RECT_PENDING;
		hw_event_wait_next(&event);
		ei_linked_event_t* to_consider = retrieve_eventtype(event.type);
		call(event, *to_consider, pick_surface);
	}
}


ei_widget_t* ei_app_root_widget(void)
{
	return  rootWidget;
}


ei_surface_t ei_app_root_surface(void)
{
	return main_window;
}
