#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"


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
	ei_size_t	frame_size		= {300,200};
	char*           text_frame              = "Salut";
	ei_anchor_t	text_anchor		= ei_anc_southwest;
	ei_color_t      text_color              = {0x45, 0x34, 0xf3, 0xff};
	int		frame_x			= 200;
	int		frame_y			= 300;
	ei_anchor_t     anchor                  = ei_anc_center;
	ei_color_t	frame_color		= {0x88, 0x88, 0x88, 0xff};
	ei_relief_t	frame_relief		= ei_relief_raised;
	int		frame_border_width	= 6;

	ei_surface_t    image;
	ei_rect_t*      rect_img                = malloc(sizeof(ei_rect_t));
	ei_anchor_t     img_anchor              = ei_anc_center;




	/* Create the application and change the color of the background. */
	ei_app_create(screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	/* Create, configure and place the frame on screen. */
	frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
        image = hw_image_load("misc/klimt.jpg", ei_app_root_surface());
        ei_rect_t  rect = hw_surface_get_rect(image);
        rect_img->size.height = rect.size.height;
        rect_img->size.width = rect.size.width;
        rect_img->top_left.x = rect.top_left.x;
        rect_img->top_left.y = rect.top_left.y;
	ei_frame_configure(frame, &frame_size, &frame_color,
			    &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL,
			    &image, &rect_img,  &img_anchor);
	ei_place(frame, &anchor, &frame_x, &frame_y, NULL, NULL, NULL, NULL, NULL, NULL );

	/* Run the application's main loop. */
	ei_app_run();

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}
