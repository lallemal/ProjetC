/******************************************************************************
* File:             ei_widgetclass.c
*
* Author:           Robin BERTIN (Nunwan)
* Created:          05/12/20
* Description:      
*****************************************************************************/
#include <stdlib.h>
#include <string.h>


#include "ei_widgetclass.h"
#include "ei_widget.h"
#include "ei_draw.h"


#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

typedef struct ei_frame {
	ei_widget_t	widget;

	ei_color_t	color;
	int		border_width;
	ei_relief_t	relief;
	char*		text;
	ei_font_t	text_font;
	ei_anchor_t	text_anchor;
	ei_color_t      text_color;
	ei_surface_t	img;
	ei_rect_t*	img_rect;
	ei_anchor_t	img_anchor;
} ei_frame_t;



void* ei_frame_allocfunc(void)
{
	void *newFrame = calloc(1, sizeof(ei_frame_t));
	return newFrame;
}


void ei_frame_releasefunc(ei_widget_t*	widget)
{
	ei_frame_t *frame = (ei_frame_t *) widget;
	//free(&(frame->color));
	//free(&(frame->border_width));
	//free(&(frame->relief));
	//free(&(frame->text_font));
	//free(&(frame->text_anchor));
	//free(&(frame->text_color));
	//free(&(frame->img));
	if (frame->img_rect != NULL) {
		free(frame->img_rect);
	}
	if (frame->text != NULL) {
		free(frame->text);
	}
	// free(&(frame->img_anchor));
}


void ei_frame_setdefaultsfunc(ei_widget_t* widget)
{
        ei_frame_t* frame= (ei_frame_t*) widget;
        frame->text = NULL;
        frame->color = ei_default_background_color;
        frame->border_width=0;
        frame->relief = ei_relief_none;
        frame->text_font = ei_default_font;
        frame->text_anchor = ei_anc_center;
        frame->text_color = ei_font_default_color;
        frame->img = NULL;
        frame->img_rect = NULL;
        frame->img_anchor = ei_anc_center;
	frame->widget.screen_location.top_left.x = 0;
	frame->widget.screen_location.top_left.y = 0;
}


/**
 * @brief	Fonction to know where is the point at the top right corner in the rectangle
 *
 * @param       surface         Global surface of the frame (in the case where clipper is NULL => rect_to_fill is NULL)
 * @param	rect		Global rectangle which will contain the text or the image
 * @param	anchor		The anchor link to the text or the image.
 * @param	width_text	width of the text or the image
 * @param	height_text	height of the text or the image
 *
 * @return			The point at the top right of the text or the image
 */
ei_point_t* anchor_point(ei_surface_t surface, ei_rect_t* rect, ei_anchor_t anchor, int width_text, int height_text)
{
        ei_point_t* point=malloc(sizeof(ei_point_t));
        int x_rect;
        int y_rect;
        int width;
        int height;
        if (rect==NULL){
                ei_rect_t* rect_surface=malloc(sizeof(ei_rect_t));
                *rect_surface = hw_surface_get_rect(surface);
                x_rect=rect_surface->top_left.x;
                y_rect=rect_surface->top_left.y;
                width=rect_surface->size.width;
                height=rect_surface->size.height;
        }
        else{
                x_rect=rect->top_left.x;
                y_rect=rect->top_left.y;
                width=rect->size.width;
                height=rect->size.height;
        }
        if (anchor == ei_anc_center){
                point->x=min((x_rect+x_rect+width)/2, x_rect+width-width_text);
                point->y=min((y_rect+y_rect+height)/2, y_rect+height-height_text);
        }
        if(anchor == ei_anc_north){
                point->x=min((x_rect+x_rect+width)/2, x_rect+width-width_text);
                point->y=y_rect;
        }
        if(anchor == ei_anc_northeast){
                point->x=x_rect+width-width_text;
                point->y=y_rect;
        }
        if(anchor == ei_anc_east){
                point->x=x_rect+width-width_text;
                point->y=min((y_rect+y_rect+height)/2, y_rect+height-height_text);
        }
        if(anchor == ei_anc_southeast){
                point->x=x_rect+width-width_text;
                point->y=y_rect+height-height_text;
        }
        if(anchor == ei_anc_south){
                point->x=min((x_rect+x_rect+width)/2, x_rect+width-width_text);
                point->y=y_rect+height-height_text;
        }
        if(anchor == ei_anc_southwest){
                point->x=x_rect;
                point->y=y_rect+height-height_text;
        }
        if(anchor == ei_anc_west){
                point->x=x_rect;
                point->y=min((y_rect+y_rect+height)/2, y_rect+height-height_text);
        }
        if(anchor == ei_anc_northwest){
                point->x=x_rect;
                point->y=y_rect;
        }
        return point;
}

/**
 * @brief	Fonction which give a clearer color fr the relief
 *
 * @param       color         actual color of the frame
 *
 * @return		        a clearer color of the actual color of the frame
 */

ei_color_t clear_color(ei_color_t color)
{
        ei_color_t color_clearer;
        if (color.blue>10) {
                color_clearer.blue = color.blue - 10;
        }
        else{
                color_clearer.blue = color.blue;
        }
        if (color.green>10) {
                color_clearer.green = color.green - 10;
        }
        else{
                color_clearer.green = color.green;
        }
        if (color.red>10) {
                color_clearer.red = color.red - 10;
        }
        else{
                color_clearer.red = color.red;
        }
        color_clearer.alpha=color.alpha;
        return color_clearer;
}

/**
 * @brief	Fonction which give a darker color fr the relief
 *
 * @param       color         actual color of the frame
 *
 * @return		        a darker color of the actual color of the frame
 */

ei_color_t dark_color(ei_color_t color)
{
        ei_color_t color_darker;
        if (color.blue<254) {
                color_darker.blue = color.blue + 10;
        }
        else{
                color_darker.blue = color.blue;
        }
        if (color.green<254) {
                color_darker.green = color.green + 10;
        }
        else{
                color_darker.green = color.green;
        }
        if (color.red<254) {
                color_darker.red = color.red + 10;
        }
        else{
                color_darker.red = color.red;
        }
        color_darker.alpha=color.alpha;
        return color_darker;
}

/**
 * @brief	Fonction which draw up and down of the rect to make a relief.
 *
 * @param       rect_to_fill    rectangle in whiwh there is the separatition in two colors to make the relief
 * @param	surface		Global surface of the frame
 * @param	frame		frame which has relief
 * @param	clipper 	clipper of the rect_to_fill
 * @param	clear_up	booleen which means if we want the clearer color on the top (depends on the value of relief)
 *
 */
void draw_up_and_down_relief(ei_rect_t* rect_to_fill, ei_surface_t surface, ei_color_t color, ei_rect_t* clipper, ei_bool_t clear_up){

        //up
        ei_linked_point_t *list_point_up1 = malloc(sizeof(ei_linked_point_t));
        ei_linked_point_t *list_point_up2 = malloc(sizeof(ei_linked_point_t));
        ei_linked_point_t *list_point_up3 = malloc(sizeof(ei_linked_point_t));

        ei_point_t first_point_up = rect_to_fill->top_left;
        ei_point_t second_point_up = *anchor_point(surface, rect_to_fill, ei_anc_southwest,
                                                   rect_to_fill->size.width,
                                                   rect_to_fill->size.height);
        ei_point_t third_point_up = *anchor_point(surface, rect_to_fill, ei_anc_northeast,
                                                  rect_to_fill->size.width,
                                                  rect_to_fill->size.height);
        list_point_up1->point = first_point_up;
        list_point_up2->point = second_point_up;
        list_point_up3->point = third_point_up;
        list_point_up1->next = list_point_up2;
        list_point_up2->next = list_point_up3;
        list_point_up3->next = NULL;
        if (clear_up == EI_TRUE) {
                ei_draw_polygon(rect_to_fill, list_point_up1, clear_color(color), clipper);
        }
        else{
                ei_draw_polygon(rect_to_fill, list_point_up1, dark_color(color), clipper);
        }
        free(list_point_up1);
        free(list_point_up2);
        free(list_point_up3);

        //down
        ei_linked_point_t *list_point_down1 = malloc(sizeof(ei_linked_point_t));
        ei_linked_point_t *list_point_down2 = malloc(sizeof(ei_linked_point_t));
        ei_linked_point_t *list_point_down3 = malloc(sizeof(ei_linked_point_t));

        ei_point_t first_point_down =  *anchor_point(surface, rect_to_fill, ei_anc_southeast,
                                                     rect_to_fill->size.width,
                                                     rect_to_fill->size.height);
        ei_point_t second_point_down = *anchor_point(surface, rect_to_fill, ei_anc_southwest,
                                                     rect_to_fill->size.width,
                                                     rect_to_fill->size.height);
        ei_point_t third_point_down = *anchor_point(surface, rect_to_fill, ei_anc_northeast,
                                                    rect_to_fill->size.width,
                                                    rect_to_fill->size.height);
        list_point_down1->point = first_point_down;
        list_point_down2->point = second_point_down;
        list_point_down3->point = third_point_down;
        list_point_down1->next = list_point_down2;
        list_point_down2->next = list_point_down3;
        list_point_down3->next = NULL;
        if (clear_up==EI_TRUE) {
                ei_draw_polygon(rect_to_fill, list_point_down1, dark_color(color), clipper);
        }
        else{
                ei_draw_polygon(rect_to_fill, list_point_down1, clear_color(color), clipper);
        }
        free(list_point_down1);
        free(list_point_down2);
        free(list_point_down3);
}


void ei_frame_drawfunc(struct	ei_widget_t*	widget,
                       ei_surface_t	surface,
                       ei_surface_t	pick_surface,
                       ei_rect_t*	clipper)
{
        ei_frame_t* frame=(ei_frame_t*)widget;
        hw_surface_lock(surface);
        ei_rect_t *rect_to_fill=malloc(sizeof(ei_rect_t));
        //on prend les deux cas clipper nul et clipper non nul pour y associer un rectangle qu est concerné par les fonctions de dessins
        if (clipper == NULL){
                rect_to_fill = NULL;
        }
        else{
                int x_clipper= clipper->top_left.x;
                int y_clipper= clipper->top_left.y;
                int height_clipper = clipper->size.height;
                int width_clipper = clipper->size.width;
                int x_frame= widget->screen_location.top_left.x;
                int y_frame = widget->screen_location.top_left.y;
                int height_frame = widget->screen_location.size.height;
                int width_frame = widget->screen_location.size.width;

                rect_to_fill->top_left.x=max(x_frame, x_clipper);
                rect_to_fill->top_left.y=max(y_clipper, y_frame);
                rect_to_fill->size.width=abs(min(x_clipper+width_clipper, x_frame+width_frame)-max(x_clipper, x_frame));
                rect_to_fill->size.height=abs(max(y_frame, y_clipper)-min(y_frame+height_frame, y_clipper+height_clipper));
        }
        //On récupère la couleur du fond
        ei_color_t color_back = frame->color;
        //On récupère le point qui rattache le texte
        //Si clipper==NULL =>  rect__to_fill==NULL
        //si text=!NULL
        ei_point_t* point_text=malloc(sizeof(ei_point_t));
        int* height_text=malloc(sizeof(int));
        int* width_text = malloc(sizeof(int));
        if (frame->text != NULL){
                hw_text_compute_size(frame->text, frame->text_font, width_text, height_text);
                int64_t height_text_int= (int64_t)height_text;
                int64_t width_text_int= (int64_t)width_text;
                point_text=anchor_point(surface, rect_to_fill, frame->text_anchor, width_text_int, height_text_int);
        }
        free(height_text);
        free(width_text);
        //On récupère le point qui rattache l'image
        //Si clipper==NULL =>  rect__to_fill==NULL (pris en compte dans la fonction anchor_point)
        //si img=!NULL
        ei_point_t* point_img=malloc(sizeof(ei_point_t));
        if (frame->img != NULL){
                point_img = anchor_point(surface, rect_to_fill, frame->img_anchor, frame->img_rect->size.width, frame->img_rect->size.height);
        }

        //remplissage de rect_to_fill
        ei_fill(surface, &color_back, rect_to_fill);

        //mise en place du texte
        if (frame->text != NULL){
                ei_draw_text(surface, point_text, frame->text, frame->text_font, frame->text_color, rect_to_fill);
        }
        free(point_text);

        //mise en place de l'image
        ei_rect_t* rect_img=malloc(sizeof(ei_rect_t));
        if (frame->img != NULL){
                hw_surface_lock(frame->img);
                rect_img->top_left = *point_img;
                rect_img->size.width= min(rect_to_fill->size.width, frame->img_rect->size.width);
                rect_img->size.height=min(rect_to_fill->size.height, frame->img_rect->size.height);
                ei_copy_surface(surface, rect_img, frame->img, frame->img_rect, hw_surface_has_alpha(frame->img));
                hw_surface_unlock(frame->img);
        }
        free(point_img);

        //mise en place de la liste contenant les rectangles à update (si NULL update toute la surface)
        //pour le texte et la couleur (si il y a du texte)
        ei_linked_rect_t *liste_rect1=malloc(sizeof(ei_linked_rect_t));
        ei_linked_rect_t *liste_rect2=malloc(sizeof(ei_linked_rect_t));

        if (rect_to_fill == NULL){
                liste_rect1 = NULL;
        }
        else {
                liste_rect1->rect = *rect_to_fill;
                liste_rect1->next = NULL;

                //pour l'image (si il y a une image
                if (frame->img != NULL) {
                        liste_rect2->next->rect = *rect_img;
                        liste_rect2->next->next = NULL;
                        liste_rect1->next = liste_rect2;
                }
        }


        //Création de relief
        int border= frame->border_width;
        if (border>0) {
                if (rect_to_fill != NULL) {
                        if (frame->relief == ei_relief_raised) {
                                draw_up_and_down_relief(rect_to_fill, surface, frame->color, clipper, EI_TRUE);
                        }
                        if (frame->relief == ei_relief_sunken) {
                                draw_up_and_down_relief(rect_to_fill, surface, frame->color, clipper, EI_FALSE);
                        }
                }
                else{
                        if (frame->relief == ei_relief_raised) {
                                draw_up_and_down_relief(surface, surface, frame->color, clipper, EI_TRUE);
                        }
                        if (frame->relief == ei_relief_sunken) {
                                draw_up_and_down_relief(surface, surface, frame->color, clipper, EI_FALSE);
                        }
                }
        }


        //on unlock et update les changements
        hw_surface_unlock(surface);
        hw_surface_update_rects(surface, liste_rect1);

        //on libère la mémoire
        free(liste_rect2);
        free(liste_rect1);
        free(rect_to_fill);
        free(rect_img);
}



void ei_frame_register_class(void)
{
	ei_widgetclass_t* frame = malloc(sizeof(ei_widgetclass_t));
	strcpy(frame->name, "frame");
	frame->allocfunc = &ei_frame_allocfunc;
	frame->releasefunc = &ei_frame_releasefunc;
	frame->drawfunc = &ei_frame_drawfunc;
	frame->setdefaultsfunc = &ei_frame_setdefaultsfunc;
	frame->next = NULL;
	ei_widgetclass_register(frame);
}


void ei_frame_configure(ei_widget_t*		widget,
			ei_size_t*		requested_size,
			const ei_color_t*	color,
			int*			border_width,
			ei_relief_t*		relief,
			char**			text,
			ei_font_t*		text_font,
			ei_color_t*		text_color,
			ei_anchor_t*		text_anchor,
			ei_surface_t*		img,
			ei_rect_t**		img_rect,
			ei_anchor_t*		img_anchor)

{
	ei_frame_t* frame = (ei_frame_t *) widget;
	if (requested_size != NULL) {
		widget->screen_location.size = *requested_size;
	}
	if (color != NULL) {
		frame->color = *color;
	}
	if (border_width != NULL) {
		frame->border_width = *border_width;
	}
	if (relief != NULL) {
		frame->relief = *relief;
	}
	if (text != NULL) {
		frame->text = *text;
	}
	if (text_font != NULL) {
		frame->text_font = *text_font;
	}
	if (text_color != NULL) {
		frame->text_color = *text_color;
	}
	if (img != NULL) {
		frame->img = *img;
	}
	if (img_rect != NULL) {
		frame->img_rect = *img_rect;
	}
	if (img_anchor != NULL) {
		frame->img_anchor = *img_anchor;
	}
}
