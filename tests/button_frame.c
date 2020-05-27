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

ei_bool_t button_press2(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
        ei_color_t	root_bgcol		= {0x12, 0x7f, 0xb4, 0xff};
        ei_frame_configure(widget, NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        return EI_TRUE;
}

ei_bool_t button_release2(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
        ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};
        ei_frame_configure(widget, NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
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
        ei_size_t	screen_size		= {800, 800};
        ei_color_t	root_bgcol		= {0x25, 0x30, 0x35, 0xff};

        ei_widget_t*	button1;
        ei_size_t	button1_size		= {200,300};
        int		button1_x		= 300;
        int		button1_y		= 850;
        ei_color_t	button1_color		= {0x88, 0x00, 0x00, 0xff};
        char*		button1_title		= "Bouton avec relief relevé";
        ei_anchor_t     button1_anchor           = ei_anc_southwest;
        ei_color_t	button1_text_color	= {0x1f, 0xd0, 0x1f, 0xff};
        ei_anchor_t     button1_text_anchor      = ei_anc_north;
        int		button1_corner_radius	= 10;
        ei_relief_t	button1_relief		= ei_relief_raised;
        int		button1_border_width	= 6;
        ei_callback_t	button1_callback 	= button_press;

        ei_widget_t*    frame1;
        ei_size_t       frame1_size             = {200, 300};
        int             frame1_x                = 300;
        int             frame1_y                = -50;
        ei_color_t      frame1_color            ={0x88, 0x00, 0x00, 0xff};
        char*           frame1_title            = "frame avec relief relevé";
        ei_anchor_t     frame1_anchor           = ei_anc_northwest;
        ei_color_t      frame1_text_color       = {0x1f, 0xd0, 0x1f, 0xff};
        ei_anchor_t     frame1_text_anchor      = ei_anc_south;
        ei_relief_t     frame1_relief           = ei_relief_raised;
        int             frame1_border_width     = 6;

        ei_widget_t*	button2;
        ei_size_t	button2_size		= {300,200};
        int		button2_x		= 850;
        int		button2_y		= 700;
        ei_color_t	button2_color		= {0x00, 0x00, 0x88, 0xff};
        ei_anchor_t     button2_anchor           = ei_anc_east;
        int		button2_corner_radius	= 20;
        ei_relief_t	button2_relief		= ei_relief_sunken;
        int		button2_border_width	= 8;
        ei_callback_t	button2_callback 	= button_press;
        ei_surface_t    image_grande;
        ei_rect_t*      rect_img_grande		= malloc(sizeof(ei_rect_t));
        ei_anchor_t     anchor_image_grande     = ei_anc_center;

        ei_widget_t*    frame2;
        ei_size_t       frame2_size             = {300, 200};
        int             frame2_x                = 850;
        int             frame2_y                = -10;
        ei_color_t      frame2_color            ={0x00, 0x00, 0x88, 0xff};
        ei_anchor_t     frame2_anchor           = ei_anc_northeast;
        ei_relief_t     frame2_relief           = ei_relief_sunken;
        int             frame2_border_width     = 8;

        ei_widget_t*	button3;
        ei_size_t	button3_size		= {200,200};
        int		button3_x		= -50;
        int		button3_y		= 700;
        ei_color_t	button3_color		= {0x00, 0x88, 0x00, 0xff};
        ei_anchor_t     button3_anchor           = ei_anc_west;
        int		button3_corner_radius	= 0;
        ei_relief_t	button3_relief		= ei_relief_none;
        int		button3_border_width	= 8;
        ei_callback_t	button3_callback 	= button_press;
        ei_surface_t    image_petite;
        ei_rect_t*      rect_img_petite		= malloc(sizeof(ei_rect_t));
        ei_anchor_t     anchor_image_petite     = ei_anc_center;

        ei_widget_t*    frame3;
        ei_size_t       frame3_size             = {200, 200};
        int             frame3_x                = -10;
        int             frame3_y                = -10;
        ei_color_t      frame3_color            ={0x00, 0x88, 0x00, 0xff};
        ei_anchor_t     frame3_anchor           = ei_anc_northwest;
        ei_relief_t     frame3_relief           = ei_relief_none;
        int             frame3_border_width     = 8;

        /* Create the application and change the color of the background. */
        ei_app_create(screen_size, EI_FALSE);
        ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        /* Create, configure and place the button on screen. */
        button1 = ei_widget_create("button", ei_app_root_widget(), NULL, NULL);

        frame1 = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL );

        button2 = ei_widget_create("button", ei_app_root_widget(), NULL, NULL);
        image_grande = hw_image_load("misc/klimt.jpg", ei_app_root_surface());
        ei_rect_t  rect_g = hw_surface_get_rect(image_grande);
        rect_img_grande->size.height = rect_g.size.height;
        rect_img_grande->size.width = rect_g.size.width;
        rect_img_grande->top_left.x = rect_g.top_left.x;
        rect_img_grande->top_left.y = rect_g.top_left.y;

        frame2 = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);

        button3 = ei_widget_create("button", ei_app_root_widget(), NULL, NULL);
        image_petite = hw_image_load("misc/flag.png", ei_app_root_surface());
        ei_rect_t  rect_p = hw_surface_get_rect(image_petite);
        rect_img_petite->size.height = rect_p.size.height;
        rect_img_petite->size.width = rect_p.size.width;
        rect_img_petite->top_left.x = rect_p.top_left.x;
        rect_img_petite->top_left.y = rect_p.top_left.y;

        frame3 = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);

        ei_button_configure	(button1, &button1_size, &button1_color,
                                    &button1_border_width, &button1_corner_radius, &button1_relief, &button1_title, NULL, &button1_text_color, &button1_text_anchor,
                                    NULL, NULL, NULL, &button1_callback, NULL);
        ei_place(button1, &button1_anchor, &button1_x, &button1_y, NULL, NULL, NULL, NULL, NULL, NULL );

        ei_frame_configure(frame1, &frame1_size, &frame1_color, &frame1_border_width, &frame1_relief, &frame1_title, NULL, &frame1_text_color, &frame1_text_anchor, NULL, NULL, NULL);
        ei_place(frame1, &frame1_anchor, &frame1_x, &frame1_y, NULL, NULL, NULL, NULL, NULL, NULL);

        ei_button_configure	(button2, &button2_size, &button2_color,
                                    &button2_border_width, &button2_corner_radius, &button2_relief, NULL, NULL, NULL, NULL,
                                    &image_grande, &rect_img_grande, &anchor_image_grande, &button2_callback, NULL);
        ei_place(button2, &button2_anchor, &button2_x, &button2_y, NULL, NULL, NULL, NULL, NULL, NULL );

        ei_frame_configure(frame2, &frame2_size, &frame2_color, &frame2_border_width, &frame2_relief, NULL, NULL, NULL, NULL, &image_grande, &rect_img_grande, &anchor_image_grande );
        ei_place(frame2, &frame2_anchor, &frame2_x, &frame2_y, NULL, NULL, NULL, NULL, NULL, NULL);

        ei_button_configure	(button3, &button3_size, &button3_color,
                                    &button3_border_width, &button3_corner_radius, &button3_relief, NULL, NULL, NULL, NULL,
                                    &image_petite, &rect_img_petite, &anchor_image_petite, &button3_callback, NULL);
        ei_place(button3, &button3_anchor, &button3_x, &button3_y, NULL, NULL, NULL, NULL, NULL, NULL );

        ei_frame_configure(frame3, &frame3_size, &frame3_color, &frame3_border_width, &frame3_relief, NULL, NULL, NULL, NULL, &image_petite, &rect_img_petite, &anchor_image_petite );
        ei_place(frame3, &frame3_anchor, &frame3_x, &frame3_y, NULL, NULL, NULL, NULL, NULL, NULL);

        /* Hook the keypress callback to the event. */
        ei_bind(ei_ev_keydown,		NULL, "all", process_key, NULL);
        ei_bind(ei_ev_mouse_buttondown, NULL, "frame", button_press2, NULL);
        ei_bind(ei_ev_mouse_buttonup, NULL, "frame", button_release2, NULL);

        /* Run the application's main loop. */
        ei_app_run();

        /* We just exited from the main loop. Terminate the application (cleanup). */
        ei_unbind(ei_ev_keydown,	NULL, "all", process_key, NULL);
        ei_unbind(ei_ev_mouse_buttondown, NULL, "frame", button_press2, NULL);
        ei_unbind(ei_ev_mouse_buttonup, NULL, "frame", button_release2, NULL);
        ei_app_free();

        return (EXIT_SUCCESS);
}
