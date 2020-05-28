#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_toplevel.h"
#include "ei_radiobutton.h"

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
	ei_widget_t*	radiobutton;
	ei_widget_t*	toplevel;
	ei_size_t	frame_size		= {300,300};
	char*           text_frame              = "looooooooooooooooooooooooooooooooooooooooooooooooooooooooooooog";
	ei_anchor_t	text_anchor		= ei_anc_northwest;
        ei_color_t      window_color            ={210, 210, 210, 0xff};
	ei_color_t      text_color              = {0, 0, 0, 0xff};
	int		frame_x			= 400;
	int		frame_y			= 150;

        int		button_x		= 50;
        int		button_y		= 50;
	ei_anchor_t     anchor                  = ei_anc_northwest;
        ei_color_t bg_col = {255, 140, 0, 255};
        ei_color_t on_press = {255, 0, 0, 255};
        ei_color_t on_rel = {255, 255, 255, 255};
        ei_color_t text_col = {50, 50, 50, 0xff};
        int border = 10;
        int new_border = 4;
        int number_of_choices = 3;
        char **liste = malloc(number_of_choices * sizeof(char *));
        liste[0] = "Choix 1";
        liste[1] = "Choix 2";
        liste[2] = "Choix 3";
        char *premie = liste[1];
	/* Create the application and change the color of the background. */
	ei_app_create(screen_size, EI_FALSE);
        //ei_toplevel_configure();
	/* Create, configure and place the frame on screen. */
        toplevel = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
        radiobutton = ei_widget_create("radiobutton", toplevel, NULL, NULL);
        ei_toplevel_configure(toplevel, &frame_size, &window_color, &border, NULL, NULL, NULL, NULL);
        ei_toplevel_configure(toplevel, NULL, NULL, &new_border, NULL, NULL, NULL, NULL);
        ei_place(toplevel, &anchor, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );
        ei_radiobutton_configure(radiobutton, NULL, &liste, NULL, &number_of_choices, &text_col, &bg_col, &on_press, &on_rel);
        float rel_wid = 0.5;
        ei_place(radiobutton, &anchor, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );
//
//        ei_button_configure(button, &top_level_size, &frame_color, NULL, NULL, NULL, &text_frame, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
//        ei_place(button, &anchor, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );
        ei_bind(ei_ev_keydown, NULL, "all", process_key, NULL);
	ei_app_run();
//	ei_unbind(ei_ev_keydown, NULL, "all", process_key, NULL);
//	ei_unbind(ei_ev_keyup, NULL, "all", process_key2, NULL);
	// ei_unbind(ei_ev_mouse_buttondown, NULL, "all", button_down, NULL);

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();
        free(liste);
	return (EXIT_SUCCESS);
}
