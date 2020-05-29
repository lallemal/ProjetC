//
// Created by aymeric on 27/05/2020.
//

#ifndef PROJETC_IG_EI_RADIOBUTTON_H
#define PROJETC_IG_EI_RADIOBUTTON_H
#include "ei_widget.h"
#include "stdio.h"
#include "stdlib.h"
#define MARGIN_LEFT_RB 5
#define BORDER_SIZE 2
/**
 * \brief	A structure that stores information about a radiobutton.
 */
typedef struct ei_radio_button {
    ei_widget_t		        widget;
    ei_size_t		        requested_size;
    char**                      choices;
    char*			title;
    int                         nb_of_choices;
    ei_widget_t*                current_button;
    int                         title_height;
    int                         title_width;
    ei_color_t                  tb_color;
    ei_color_t                  bg_color;
    ei_color_t                  b_press_col;
    ei_color_t                  b_rel_col;
    ei_bool_t                   is_size_set;

} ei_radio_button;
/**
 * @brief           See the definition of \ref ei_widgetclass_allocfunc_t
 *
 */
void                    *ei_radiobutton_allofunc                                (void);
/**
 * @brief           See the definition of \ref ei_widgetclass_releasefunc_t
 *
 */
void                    ei_radiobutton_releasefunc                              (ei_widget_t* widget);
/**
 * @brief           See the definition of \ref ei_widgetclass_setdefaultsfunc_t
 *
 */
void                    ei_radiobutton_setdefaultsfunc                          (ei_widget_t* widget);
/**
 * @brief           See the definition of \ref ei_widgetclass_geomnotifyfunc_t
 *
 */
void                    ei_radiobutton_geomnotifyfunc                           (ei_widget_t* widget);
/**
 * @brief           See the definition of \ref ei_widgetclass_drawfunc_t
 *
 */
void                    ei_toplevel_drawfunc                                    (struct	ei_widget_t*	widget,
                                                                                 ei_surface_t	surface,
                                                                                 ei_surface_t	pick_surface,
                                                                                 ei_rect_t*	clipper);


/**
 * @brief	Configures the attributes of widgets of the class "radiobutton".
 *
 *		Parameters obey the "default" protocol: if a parameter is "NULL" and it has never
 *		been defined before, then a default value should be used (default values are
 *		specified for each parameter). If the parameter is "NULL" but was defined on a
 *		previous call, then its value must not be changed.
 *
 * @param	widget		        The widget to configure.
 * @param	requested_size	        The size requested for this widget, including borders.
 *				        The geometry manager may override this size due to other constraints.
 *				        Defaults to the "natural size" of the widget.
 * @param	choices		        The tab which contains all the text for all the choices
 * @param	title	                The title of your radio button
 * @param	nb_of_choices	        The number of choices (has to be equal to choices tab size)
 * @param	text_border_color       The color of the text and the border
 * @param	background_color	The color of the background
 * @param	press_col	        The color of the buttons when someone press on it
 * @param	rel_color	        The default color of the buttons
 *
 */
void                    ei_radiobutton_configure                                (ei_widget_t      *widget,
                                                                                 ei_size_t       *requested_size,
                                                                                 char            ***choices,
                                                                                 char            **title,
                                                                                 int             *nb_of_choices,
                                                                                 ei_color_t      *text_border_color,
                                                                                 ei_color_t      *background_color,
                                                                                 ei_color_t      *press_col,
                                                                                 ei_color_t      *rel_color);


/**
 * @brief	Register the class of radio button, this function just need to be called once
 */
void                    ei_radiobutton_register_class                            (void);
/**
 * @brief	Configures the size of the choices text to adapt them to requested size (truncate if too big)
 *
 * @param	text_placer	        The biggest rectangle which can contain the text
 * @param	color		        The color of the text
 * @param	surface	                The surface where it's going to be draw
 * @param	clipper	                The actual clipper
 * @param	text                    The text of this particular choice
 *
 */
void                    text_size                                       (ei_rect_t *text_placer,
                                                                         ei_color_t *color,
                                                                         ei_surface_t *surface,
                                                                         ei_rect_t *clipper,
                                                                         char* text);
/**
 * @brief	Return the actual choice in the list of schoices
 *
 * @return      The actual choice
 */
char*                   data                                            (ei_widget_t *widget);

#endif //PROJETC_IG_EI_RADIOBUTTON_H
