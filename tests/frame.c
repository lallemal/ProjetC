#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_toplevel.h"

ei_bool_t process_key(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (event->param.key.key_code == SDLK_ESCAPE) {
		ei_app_quit_request();
		return EI_TRUE;
	}
        return EI_FALSE;
}

ei_bool_t process_key2(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (event->param.key.key_code == SDLK_f) {
		ei_app_quit_request();
		return EI_TRUE;
	}
        return EI_FALSE;
}

ei_bool_t button_down(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	ei_app_quit_request();
	return EI_TRUE;
}

ei_bool_t del_widget(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (event->param.key.key_code == SDLK_DELETE) {
		ei_widget_destroy(user_param);
	}
	return EI_FALSE;
}
/*
 * main --
 *
 *	Main function of the application.
 */
int main(int argc, char** argv)
{
	ei_size_t	screen_size		= {600, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};
        ei_size_t       top_level_size          = {300, 300};
	ei_widget_t*	button;
	ei_widget_t*	toplevel;
	ei_size_t	frame_size		= {300,200};
	char*           text_frame              = "Salut";
	ei_anchor_t	text_anchor		= ei_anc_northwest;

	ei_color_t      text_color              = {0, 0, 0, 0xff};
	int		frame_x			= 15;
	int		frame_y			= 15;

        int		button_x		= 0;
        int		button_y		= 0;
	ei_anchor_t     anchor                  = ei_anc_northwest;
	ei_color_t	frame_color		= {250, 0, 0, 0xff};
	ei_color_t	frame_color_2		= {200, 200, 200, 125};
	ei_relief_t	frame_relief		= ei_relief_raised;
	int		frame_border_width	= 6;
        ei_bool_t       closable = EI_TRUE;
	ei_surface_t    image;
	ei_rect_t*      rect_img                = malloc(sizeof(ei_rect_t));
	ei_anchor_t     img_anchor              = ei_anc_center;
        ei_axis_set_t   axis                    = ei_axis_both;

	/* Create the application and change the color of the background. */
	ei_app_create(screen_size, EI_FALSE);
        //ei_toplevel_configure();
	/* Create, configure and place the frame on screen. */
	toplevel = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
//        button = ei_widget_create("button", ei_app_root_widget(), NULL, NULL);
//
        ei_toplevel_configure(toplevel, &top_level_size, &frame_color_2, NULL, &text_frame, &closable, &axis, NULL);
        ei_place(toplevel, &anchor, &frame_x, &frame_y, NULL, NULL, NULL, NULL, NULL, NULL );

//        ei_button_configure(button, &top_level_size, &frame_color, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
//        toplevel->content_rect = &toplevel->screen_location;
//        ei_place(button, &anchor, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );

	ei_app_run();
//	ei_unbind(ei_ev_keydown, NULL, "all", process_key, NULL);
//	ei_unbind(ei_ev_keyup, NULL, "all", process_key2, NULL);
	// ei_unbind(ei_ev_mouse_buttondown, NULL, "all", button_down, NULL);

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}
