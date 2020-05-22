#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_toplevel.h"

/*
 * main --
 *
 *	Main function of the application.
 */
int main(int argc, char** argv)
{
	ei_size_t	screen_size		= {600, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_widget_t*	frame;
	ei_widget_t*	toplevel;
	ei_size_t	frame_size		= {300,200};
	char*           text_frame              = "Salut";
	ei_anchor_t	text_anchor		= ei_anc_northwest;
	ei_color_t      text_color              = {0, 0, 0, 0xff};
	int		frame_x			= 450;
	int		frame_y			= 450;
	ei_anchor_t     anchor                  = ei_anc_center;
	ei_color_t	frame_color		= {0x88, 0x88, 0x88, 0xff};
	ei_relief_t	frame_relief		= ei_relief_raised;
	int		frame_border_width	= 6;

	ei_surface_t    image;
	ei_rect_t*      rect_img                = malloc(sizeof(ei_rect_t));
	ei_anchor_t     img_anchor              = ei_anc_center;

	/* Create the application and change the color of the background. */
	ei_app_create(screen_size, EI_FALSE);
        //ei_toplevel_configure();
	/* Create, configure and place the frame on screen. */
	toplevel = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);


        ei_toplevel_configure(toplevel, NULL, &frame_color, NULL, NULL, NULL, NULL, NULL);
        ei_place(toplevel, &anchor, &frame_x, &frame_y, NULL, NULL, NULL, NULL, NULL, NULL );

	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}
