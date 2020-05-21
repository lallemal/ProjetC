#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"


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

	ei_widget_t*	frame;
	ei_size_t	frame_size		= {300,200};
	char*           text_frame              = "Salut";
	ei_anchor_t	text_anchor		= ei_anc_northwest;
	ei_color_t      text_color              = {0x45, 0x34, 0xf3, 0xff};
	int		frame_x			= -20;
	int		frame_y			= 0;
	ei_anchor_t     anchor                  = ei_anc_northwest;
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
			    &frame_border_width, &frame_relief, &text_frame, NULL, &text_color, &text_anchor,
			    NULL, &rect_img,  &img_anchor);
	ei_place(frame, &anchor, &frame_x, &frame_y, NULL, NULL, NULL, NULL, NULL, NULL );

	ei_widget_t*	frame2 = ei_widget_create("frame", frame, NULL, NULL);
	ei_size_t	frame2_size		= {100,50};
	float		frame2_x			= 0.5;
	float		frame2_y			= 0.5;
	ei_color_t	frame2_color		= {0x11, 0x88, 0x88, 0xff};
	int		frame2_border_width	= 1;
	ei_anchor_t     anchor2                  = ei_anc_center;

	ei_frame_configure(frame2, &frame2_size, &frame2_color,
			    &frame2_border_width, &frame_relief, NULL, NULL, NULL, NULL,
			    NULL, NULL, NULL);
	ei_place(frame2, &anchor2, NULL, NULL, NULL, NULL, &frame2_x, &frame2_y, NULL, NULL );
	ei_bind(ei_ev_keydown, NULL, "all", process_key, NULL);
	ei_bind(ei_ev_keyup, NULL, "all", process_key2, NULL);
	ei_bind(ei_ev_keydown, frame2, NULL, del_widget, frame2);
	// ei_bind(ei_ev_mouse_buttondown, NULL, "all", button_down, NULL);

	/* Run the application's main loop. */
	ei_app_run();
	ei_unbind(ei_ev_keydown, NULL, "all", process_key, NULL);
	ei_unbind(ei_ev_keyup, NULL, "all", process_key2, NULL);
	// ei_unbind(ei_ev_mouse_buttondown, NULL, "all", button_down, NULL);

	/* We just exited from the main loop. Terminate the application (cleanup). */
	ei_app_free();

	return (EXIT_SUCCESS);
}
