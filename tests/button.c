#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"


/*
 * button_press --
 *
 *	Callback called when a user clicks on the button.
 */
ei_bool_t button_press(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	printf("Click !\n");
	return EI_TRUE;
}

/*
 * process_key --
 *
 *	Callback called when any key is pressed by the user.
 *	Simply looks for the "Escape" key to request the application to quit.
 */
ei_bool_t process_key(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
	if (event->param.key.key_code == SDLK_ESCAPE) {
		ei_app_quit_request();
		return EI_TRUE;
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

	ei_widget_t*	button;
	ei_size_t	button_size		= {300,200};
	int		button_x		= 500;
	int		button_y		= 250;
	ei_color_t	button_color		= {0x88, 0x88, 0x88, 0xff};
	char*		button_title		= "Mon premier Bouton !";
	ei_anchor_t     button_anchor           = ei_anc_center;
	ei_color_t	button_text_color	= {0x00, 0x00, 0x00, 0xff};
	ei_anchor_t     button_text_anchor      = ei_anc_northeast;
	int		button_corner_radius	= 20;
	ei_relief_t	button_relief		= ei_relief_raised;
	int		button_border_width	= 6;
	ei_callback_t	button_callback 	= button_press;
	ei_surface_t    image;
	ei_rect_t*      rect_img;
	ei_anchor_t     anchor_image            = ei_anc_east;

	/* Create the application and change the color of the background. */
	ei_app_create(screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	/* Create, configure and place the button on screen. */
	button = ei_widget_create("button", ei_app_root_widget(), NULL, NULL);
        image = hw_image_load("misc/flag.png", ei_app_root_surface());
        ei_rect_t  rect = hw_surface_get_rect(image);
        rect_img->size.height = rect.size.height;
        rect_img->size.width = rect.size.width;
        rect_img->top_left.x = rect.top_left.x;
        rect_img->top_left.y = rect.top_left.y;
	ei_button_configure	(button, &button_size, &button_color,
				 &button_border_width, &button_corner_radius, &button_relief, NULL, NULL, NULL, NULL,
				 &image, &rect_img, &anchor_image, &button_callback, NULL);
	ei_place(button, &button_anchor, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );

	/* Hook the keypress callback to the event. */
	//ei_bind(ei_ev_keydown,		NULL, "all", process_key, NULL);

	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	//ei_unbind(ei_ev_keydown,	NULL, "all", process_key, NULL);
	ei_app_free();

	return (EXIT_SUCCESS);
}
