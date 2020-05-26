/******************************************************************************
* File:             draw_tools.c
*
* Author:           Robin Bertin, Aymeric Devriésère, Louise Lallemand
* Created:          05/15/20
* Description:	    Tools for drawing widget : text, img, color.
*****************************************************************************/
#include <stdlib.h>
#include <string.h>


#include "ei_widgetclass.h"
#include "ei_widget.h"
#include "ei_draw.h"
#include "math.h"
#include "draw_tools.h"
#include "utils.h"
#include "ei_types.h"


#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

void free_linked_point_list(ei_linked_point_t* begin)
{
        if (begin!=NULL){
                ei_linked_point_t* next= begin->next;
                free(begin);
                free_linked_point_list(next);
        }
}

ei_point_t* anchor_point( ei_rect_t* rect, ei_anchor_t anchor, int width_text, int height_text)
{
        ei_point_t* point=malloc(sizeof(ei_point_t));
        int x_rect;
        int y_rect;
        int width;
        int height;
        x_rect=rect->top_left.x;
        y_rect=rect->top_left.y;
        width=rect->size.width;
        height=rect->size.height;

        //for each anchor, point's coordinates are calculated in order to give the good coordinate for the top left point in rect

        if (anchor == ei_anc_center){
                point->x=(x_rect+x_rect+width)/2 - width_text/2;
                point->y=(y_rect+y_rect+height)/2 - height_text/2;
        }
        if(anchor == ei_anc_north){
                point->x=(x_rect+x_rect+width)/2 - width_text/2;
                point->y=y_rect;
        }
        if(anchor == ei_anc_northeast){
                point->x=x_rect+width-width_text;
                point->y=y_rect;
        }
        if(anchor == ei_anc_east){
                point->x=x_rect+width-width_text;
                point->y=(y_rect+y_rect+height)/2 - height_text/2;
        }
        if(anchor == ei_anc_southeast){
                point->x=x_rect+width-width_text;
                point->y=y_rect+height-height_text;
        }
        if(anchor == ei_anc_south){
                point->x=(x_rect+x_rect+width)/2 - width_text/2;
                point->y=y_rect+height-height_text;
        }
        if(anchor == ei_anc_southwest){
                point->x=x_rect;
                point->y=y_rect+height-height_text;
        }
        if(anchor == ei_anc_west){
                point->x=x_rect;
                point->y=(y_rect+y_rect+height)/2 - height_text/2;
        }
        if(anchor == ei_anc_northwest){
                point->x=x_rect;
                point->y=y_rect;
        }
        return point;
}

ei_color_t dark_color(ei_color_t color)
{
        //for each sub color in color, substraction for darker color. If it's not possible, the sub color stay the same.
        ei_color_t color_darker;
        if (color.blue>20) {
                color_darker.blue = color.blue - 20;
        }
        else{
                color_darker.blue = color.blue;
        }
        if (color.green>20) {
                color_darker.green = color.green - 20;
        }
        else{
                color_darker.green = color.green;
        }
        if (color.red>20) {
                color_darker.red = color.red - 20;
        }
        else{
                color_darker.red = color.red;
        }
        color_darker.alpha=color.alpha;
        return color_darker;
}


ei_color_t clear_color(ei_color_t color)
{
        //for each sub color in color, addition for darker color. If it's not possible the sub color stay the same.
        ei_color_t color_clearer;
        if (color.blue<235) {
                color_clearer.blue = color.blue + 20;
        }
        else{
                color_clearer.blue = color.blue;
        }
        if (color.green<235) {
                color_clearer.green = color.green + 20;
        }
        else{
                color_clearer.green = color.green;
        }
        if (color.red<235) {
                color_clearer.red = color.red + 20;
        }
        else{
                color_clearer.red = color.red;
        }
        color_clearer.alpha=color.alpha;
        return color_clearer;
}


void draw_up_relief(ei_rect_t* rect_to_fill, ei_surface_t surface, ei_color_t color, ei_bool_t clear_up, ei_rect_t* clipper)
{
        //The relief (part up) is composed of 5 points
        ei_linked_point_t *list_point_up1 = malloc(sizeof(ei_linked_point_t));
        ei_linked_point_t *list_point_up2 = malloc(sizeof(ei_linked_point_t));
        ei_linked_point_t *list_point_up3 = malloc(sizeof(ei_linked_point_t));
        ei_linked_point_t *list_point_up4 = malloc(sizeof(ei_linked_point_t));
        ei_linked_point_t *list_point_up5 = malloc(sizeof(ei_linked_point_t));
        ei_point_t first_point_up;
        ei_point_t second_point_up;
        ei_point_t third_point_up;
        ei_point_t fourth_point_up;
        ei_point_t fifth_point_up;

        first_point_up.x = rect_to_fill->top_left.x;
        first_point_up.y = rect_to_fill->top_left.y;
        second_point_up.x = rect_to_fill->top_left.x + rect_to_fill->size.width;
        second_point_up.y = rect_to_fill->top_left.y ;
        third_point_up.x = rect_to_fill->top_left.x + rect_to_fill->size.width-min(rect_to_fill->size.height/2, rect_to_fill->size.width/2);
        third_point_up.y = rect_to_fill->top_left.y + min(rect_to_fill->size.height/2, rect_to_fill->size.width/2);
        fourth_point_up.x =rect_to_fill->top_left.x + min(rect_to_fill->size.height/2, rect_to_fill->size.width/2);
        fourth_point_up.y = rect_to_fill->top_left.y + rect_to_fill->size.height - min(rect_to_fill->size.height/2, rect_to_fill->size.width/2);
        fifth_point_up.x = rect_to_fill->top_left.x;
        fifth_point_up.y = rect_to_fill->top_left.y + rect_to_fill->size.height;

        list_point_up1->point = first_point_up;
        list_point_up2->point = second_point_up;
        list_point_up3->point = third_point_up;
        list_point_up4->point = fourth_point_up;
        list_point_up5->point = fifth_point_up;
        list_point_up1->next = list_point_up2;
        list_point_up2->next = list_point_up3;
        list_point_up3->next = list_point_up4;
        list_point_up4->next = list_point_up5;
        list_point_up5->next = NULL;

        //place the clearer color on the top or the bottom according to the bool clear_up
        if (clear_up==EI_TRUE) {
                ei_draw_polygon(surface, list_point_up1, clear_color(color), clipper);
        }
        if (clear_up==EI_FALSE){
                ei_draw_polygon(surface, list_point_up1, dark_color(color), clipper);
        }
        free_linked_point_list(list_point_up1);



}

void draw_down_relief(ei_rect_t* rect_to_fill, ei_surface_t surface, ei_color_t color, ei_bool_t clear_up, ei_rect_t* clipper)
{
        //The relief (part down) is composed of 5 points
        ei_linked_point_t *list_point_down1 = malloc(sizeof(ei_linked_point_t));
        ei_linked_point_t *list_point_down2 = malloc(sizeof(ei_linked_point_t));
        ei_linked_point_t *list_point_down3 = malloc(sizeof(ei_linked_point_t));
        ei_linked_point_t *list_point_down4 = malloc(sizeof(ei_linked_point_t));
        ei_linked_point_t *list_point_down5 = malloc(sizeof(ei_linked_point_t));

        ei_point_t first_point_down;
        ei_point_t second_point_down;
        ei_point_t third_point_down;
        ei_point_t fourth_point_down;
        ei_point_t fifth_point_down;
        first_point_down.x = rect_to_fill->top_left.x + rect_to_fill->size.width;
        first_point_down.y = rect_to_fill->top_left.y + rect_to_fill->size.height;
        second_point_down.x = rect_to_fill->top_left.x + rect_to_fill->size.width;
        second_point_down.y = rect_to_fill->top_left.y ;
        third_point_down.x = rect_to_fill->top_left.x + rect_to_fill->size.width-min(rect_to_fill->size.height/2, rect_to_fill->size.width/2);
        third_point_down.y = rect_to_fill->top_left.y + min(rect_to_fill->size.height/2, rect_to_fill->size.width/2);
        fourth_point_down.x =rect_to_fill->top_left.x+ min(rect_to_fill->size.height/2, rect_to_fill->size.width/2);
        fourth_point_down.y = rect_to_fill->top_left.y + rect_to_fill->size.height - min(rect_to_fill->size.height/2, rect_to_fill->size.width/2);
        fifth_point_down.x = rect_to_fill->top_left.x;
        fifth_point_down.y = rect_to_fill->top_left.y + rect_to_fill->size.height;

        list_point_down1->point = first_point_down;
        list_point_down2->point = second_point_down;
        list_point_down3->point = third_point_down;
        list_point_down4->point = fourth_point_down;
        list_point_down5->point = fifth_point_down;
        list_point_down1->next = list_point_down2;
        list_point_down2->next = list_point_down3;
        list_point_down3->next = list_point_down4;
        list_point_down4->next = list_point_down5;
        list_point_down5->next = NULL;

        //place the darker color on the top or the bottom according to the bool clear_up
        if (clear_up==EI_TRUE) {
                ei_draw_polygon(surface, list_point_down1, dark_color(color), clipper);
        }
        if (clear_up==EI_FALSE){
                ei_draw_polygon(surface, list_point_down1, clear_color(color), clipper);
        }
        free_linked_point_list(list_point_down1);


}

void draw_text(char* text, ei_font_t text_font, ei_rect_t* rect_to_fill, ei_anchor_t text_anchor, ei_surface_t surface, ei_color_t text_color, ei_rect_t* clipper)
{
        ei_point_t* point_text;
        //set up of the height and width of the text
        int height_text;
        int width_text;
        hw_text_compute_size(text, text_font, &width_text, &height_text);
        //determine the coordinates of the text in the rect_to_fill
        point_text = anchor_point(rect_to_fill, text_anchor, width_text,height_text);
        //recto_to_fill_on_screen = intersection between rect_to_fill and the clipper (rectangle visible on screen)
        ei_rect_t* rect_to_fill_on_screen = malloc(sizeof(ei_rect_t));
        *rect_to_fill_on_screen = inter_rect(clipper, rect_to_fill);
        //draw the text
        ei_draw_text(surface, point_text, text, text_font, text_color, rect_to_fill_on_screen);
        free(point_text);
        free(rect_to_fill_on_screen);
}

void draw_image(ei_surface_t image, ei_rect_t* rect_to_fill, ei_anchor_t img_anchor, ei_rect_t* img_rect, ei_rect_t* clipper, ei_surface_t surface)
{
        ei_point_t* point_img;
        //set up of the possible case where img_rect is NULL (in this case rectangle is the general rectangle of the image)
        ei_rect_t* rectangle;
        if (img_rect == NULL){
                rectangle = malloc(sizeof(ei_rect_t));
               *rectangle = hw_surface_get_rect(image);
        }
        else{
                rectangle = copy_rect(img_rect);
        }

        point_img = anchor_point(rect_to_fill, img_anchor, rectangle->size.width, rectangle->size.height);
        hw_surface_lock(image);
        //source_rectangle = the rectangle of the image that we will copy
        ei_rect_t* source_rectangle = copy_rect(rectangle);
        //rect_img = destination of the copy of the image
        ei_rect_t* rect_img = malloc(sizeof(ei_rect_t));
        //rect_to_fill_on_screen = intersection between rect_to_fill and the clipper (rectangle visible on screen)
        ei_rect_t* rect_to_fill_on_screen = malloc(sizeof(ei_rect_t));
        *rect_to_fill_on_screen = inter_rect(rect_to_fill, clipper);

        //case where the image is smaller than the rect_to_fill, in this case the rect_img take the size of the image
        if (rectangle->size.height < rect_to_fill->size.height && rectangle->size.width < rect_to_fill->size.width){
                rect_img->top_left.x = max(0, point_img->x);
                rect_img->top_left.y = max(0,point_img->y);
                rect_img->size.width = rectangle->size.width;
                rect_img->size.height = rectangle->size.height;
                if (point_img->x < 0) {
                        source_rectangle->top_left.x = rectangle->top_left.x + abs(point_img->x);
                        source_rectangle->size.width = source_rectangle->size.width - source_rectangle->top_left.x;
                        rect_img->size.width = source_rectangle->size.width;
                }
                if (point_img->y < 0) {
                        source_rectangle->top_left.y = rectangle->top_left.y + abs(point_img->y);
                        source_rectangle->size.height = source_rectangle->size.height - source_rectangle->top_left.y;
                        rect_img->size.height = source_rectangle->size.height;
                }

                //if all is in positive coordinates, source_rectangle is all the image (NULL)
                if (point_img->x >= 0 && point_img->y >= 0) {
                        free(source_rectangle);
                        source_rectangle = NULL;
                }
        }
        //case where the image is bigger than the rect_to_fill, in this case the source rectangle take the size of the rect_to_fill_on_screen
        else {
                rect_img->top_left.x = max(rect_to_fill_on_screen->top_left.x,point_img->x);
                rect_img->top_left.y = max(rect_to_fill_on_screen->top_left.y, point_img->y);
                rect_img->size.width = rect_to_fill_on_screen->size.width;
                rect_img->size.height = rect_to_fill_on_screen->size.height;
                if (point_img->x < 0) {
                        source_rectangle->top_left.x = rectangle->top_left.x + abs(point_img->x);
                        source_rectangle->size.width = rect_img->size.width - source_rectangle->top_left.x;


                }
                if (point_img->y < 0) {
                        source_rectangle->top_left.y = rectangle->top_left.y + abs(point_img->y);
                        source_rectangle->size.height = rect_img->size.height - source_rectangle->top_left.y;
                }


                if (point_img->x >= 0) {
                        source_rectangle->top_left.x = img_rect->top_left.x;
                        source_rectangle->size.width = rect_img->size.width;
                }
                if (point_img->y >= 0){
                        source_rectangle->top_left.y = img_rect->top_left.y;
                        source_rectangle->size.height = rect_img->size.height;
                }
        }
        ei_copy_surface(surface, rect_img, image, source_rectangle, hw_surface_has_alpha(image));

        hw_surface_unlock(image);
        free(point_img);

        free(rectangle);
        free(source_rectangle);
        free(rect_to_fill_on_screen);
        free(rect_img);
}



// Add a point x, y to a linked list by tail bc of order
void add_point_list(ei_linked_point_t** begin_pt, int x, int y)
{
	ei_linked_point_t* begin = *begin_pt;
	ei_linked_point_t* newElement = malloc(sizeof(ei_linked_point_t));
	newElement->point.x = x;
	newElement->point.y = y;
	if (begin == NULL) {
		*begin_pt = newElement;
		newElement->next = NULL;
		return;
	}
	while (begin->next != NULL) {
		begin = begin->next;
	}
	begin->next = newElement;
	newElement->next = NULL;
}


void add_points_coin	(ei_linked_point_t**	begin_pt,
			int 			x_center,
			int			y_center,
			int			radius,
			float			sub_length,
			float                   corner_begin
			)
{
        //loop which add points in order to draw an arc
        for (int i=1; i<11; i ++) {
                add_point_list(begin_pt, (int) (x_center + radius * cos(corner_begin - i*sub_length/radius)),
                               (int) (y_center - radius * sin(corner_begin - i*sub_length/radius)));
        }
}






ei_linked_point_t* arc_point(ei_point_t center, int radius, float corner_begin, float corner_end)
{
        int x_center = center.x;
        int y_center = center.y;
        ei_linked_point_t* list_head = NULL;


        //case where points will be in the positives values of cosinus (right of the trigo circle)
        if ((float)cos(corner_begin)==1 || (float)cos(corner_end)==1){
                //case where points will be in the positives values of sinus (top right)
                if ((float)sin(corner_end)==1 || (float)sin(corner_begin)==1){
                        //length = length of the arc, will be divide in order to have 10 points on the arc
                        float length = (corner_end - corner_begin)*radius;
                        float sub_length = -length/10;
                        //first point of the arc
			add_point_list(&list_head, x_center+radius, y_center);
			//arc
			add_points_coin(&list_head, x_center, y_center, radius, sub_length, corner_begin);
			//last point of the arc
			add_point_list(&list_head, x_center, y_center - radius);
                }
                //case where points will be in the negatives values of sinus (bottom right)
                if ((float)sin(corner_end)==-1 || (float)sin(corner_begin)==-1){
                        float length = (corner_begin - corner_end)*radius;
                        float sub_length = length/10;
			add_point_list(&list_head, x_center, y_center +radius);
			add_points_coin(&list_head, x_center, y_center, radius,sub_length, corner_begin);
			add_point_list(&list_head, x_center+radius, y_center);
                }

        }
        //case where points will be in the negatives values of the cosinus (left of the trigo circle)
        else{
                //case where points will be in the positives values of sinus (top left)
                if ((float)sin(corner_begin)==1 || (float)sin(corner_end)==1){
                        float length = -(corner_end - corner_begin)*radius;
                        float sub_length = length/10;
			add_point_list(&list_head, x_center, y_center - radius);
			add_points_coin(&list_head, x_center, y_center, radius, sub_length, corner_begin);
			add_point_list(&list_head, x_center - radius, y_center);
                }
                //case where points will be in the negatives values of sinus (bottom left)
                if ((float)sin(corner_end) == -1 || (float)sin(corner_begin)==-1){
                        float length = -(corner_end - corner_begin)*radius;
                        float sub_length = length/10;
			add_point_list(&list_head, x_center - radius, y_center);
			add_points_coin(&list_head, x_center, y_center, radius, sub_length, corner_begin);
			add_point_list(&list_head, x_center, y_center + radius);
                }
        }
        return list_head;
}


void fusion_2_list(ei_linked_point_t* list1, ei_linked_point_t* list2, int tokeep_1)
{
	int i = 0;
	ei_linked_point_t* to_link_1 = list1;
	while (to_link_1 != NULL && i < tokeep_1 - 1) {
		to_link_1 = to_link_1->next;
		i++;
	}
	if (to_link_1 != NULL) {
		free_linked_point_list(to_link_1->next);
		to_link_1->next = list2;
	}
}

ei_linked_point_t*  rounded_top_level(ei_rect_t* rect, int radius, int part){
        ei_point_t center_top_left;
        center_top_left.x = rect->top_left.x + radius;
        center_top_left.y = rect->top_left.y + radius;
        ei_point_t center_top_right;
        center_top_right.x = rect->top_left.x + rect->size.width - radius;
        center_top_right.y = rect->top_left.y + radius;

        ei_point_t center_bottom_left;
        center_bottom_left.x = rect->top_left.x ;
        center_bottom_left.y = rect->top_left.y + rect->size.height;
        ei_point_t center_bottom_right;
        center_bottom_right.x = rect->top_left.x + rect->size.width ;
        center_bottom_right.y = rect->top_left.y + rect->size.height;
        ei_linked_point_t *rounded_top_left = arc_point(center_top_left, radius, -3*M_PI/2, -M_PI);
        ei_linked_point_t *rounded_top_right = arc_point(center_top_right, radius, -2*M_PI, -3*M_PI/2);
        add_point_list(&rounded_top_left, center_bottom_left.x, center_bottom_right.y);
        add_point_list(&rounded_top_left, center_bottom_right.x, center_bottom_right.y);
        fusion_2_list(rounded_top_left, rounded_top_right, 14);


        return rounded_top_left;

}

ei_linked_point_t* rounded_frame(ei_rect_t* rect, int radius, int part)
{
        //center of different corner for the rounded frame
        ei_point_t center_top_left;
        center_top_left.x = rect->top_left.x + radius;
        center_top_left.y = rect->top_left.y + radius;
        ei_point_t center_top_right;
        center_top_right.x = rect->top_left.x + rect->size.width - radius;
        center_top_right.y = rect->top_left.y + radius;
        ei_point_t center_bottom_left;
        center_bottom_left.x = rect->top_left.x + radius;
        center_bottom_left.y = rect->top_left.y + rect->size.height - radius;
        ei_point_t center_bottom_right;
        center_bottom_right.x = rect->top_left.x + rect->size.width - radius;
        center_bottom_right.y = rect->top_left.y + rect->size.height - radius;

        //case where the button is only a circle (less points are necessary)
        if (rect->size.width == 2*radius && rect->size.height == 2*radius){
                ei_linked_point_t *rounded_all = NULL ;
                add_point_list(&rounded_all, center_bottom_left.x + radius, center_bottom_left.y);
                add_point_list(&rounded_all, round(center_bottom_left.x + radius * cos(-7*M_PI/4)), round(center_bottom_left.y - radius * sin(-7*M_PI/4)));
                add_point_list(&rounded_all, center_bottom_left.x, center_bottom_left.y - radius);
                add_point_list(&rounded_all, round(center_bottom_left.x + radius * cos(-5*M_PI/4)), round(center_bottom_left.y - radius * sin(-5*M_PI/4)));
                add_point_list(&rounded_all, center_bottom_left.x - radius, center_bottom_left.y);
                add_point_list(&rounded_all, round(center_bottom_left.x + radius * cos(-3*M_PI/4)), round(center_bottom_left.y - radius * sin(-3*M_PI/4)));
                add_point_list(&rounded_all, center_bottom_left.x, center_bottom_left.y + radius);
                add_point_list(&rounded_all, round(center_bottom_left.x + radius * cos(-M_PI/4)), round(center_bottom_left.y - radius * sin(-M_PI/4)));
                return rounded_all;
        }
        else {
                //list of each corner of the rounded frame
                ei_linked_point_t *rounded_top_left = arc_point(center_top_left, radius, -3 * M_PI / 2, -M_PI);
                ei_linked_point_t *rounded_top_right = arc_point(center_top_right, radius, -2 * M_PI, -3 * M_PI / 2);
                ei_linked_point_t *rounded_bottom_left = arc_point(center_bottom_left, radius, -M_PI, -M_PI / 2);
                ei_linked_point_t *rounded_bottom_right = arc_point(center_bottom_right, radius, (-M_PI) / 2, 0);
                //part==0  => no relief needed
                if (part == 0) {
                        fusion_2_list(rounded_top_left, rounded_bottom_left, 12);
                        fusion_2_list(rounded_bottom_left, rounded_bottom_right, 12);
                        fusion_2_list(rounded_bottom_right, rounded_top_right, 12);
                        return rounded_top_left;
                }
                else {
                        //two point in the middle to draw relief
                        ei_point_t point_inter_bas;
                        ei_point_t point_inter_haut;
                        point_inter_haut.x =
                                rect->top_left.x + rect->size.width - min(rect->size.height / 2, rect->size.width / 2);
                        point_inter_haut.y = rect->top_left.y + min(rect->size.height / 2, rect->size.width / 2);
                        point_inter_bas.x = rect->top_left.x + min(rect->size.height / 2, rect->size.width / 2);
                        point_inter_bas.y =
                                rect->top_left.y + rect->size.height - min(rect->size.height / 2, rect->size.width / 2);
                        ei_linked_point_t *inter_list = NULL;

                        //part == 1  => ei_relief_raised
                        if (part == 1) {
                                fusion_2_list(rounded_top_right, rounded_top_left, 12);
                                fusion_2_list(rounded_top_left, rounded_bottom_left, 12);
                                add_point_list(&inter_list, point_inter_bas.x, point_inter_bas.y);
                                add_point_list(&inter_list, point_inter_haut.x, point_inter_haut.y);
                                //fusion inter_list and rounded_bottom_left only from the middle
                                fusion_2_list(rounded_bottom_left, inter_list, 5);
                                ei_linked_point_t *rounded_up = rounded_top_right->next->next->next->next;
                                rounded_top_right->next->next->next->next = NULL;
                                free_linked_point_list(rounded_top_right);
                                free_linked_point_list(rounded_bottom_right);
                                return rounded_up;
                        }
                        //part == 2 => ei_relief_sunken
                        else {
                                fusion_2_list(rounded_bottom_left, rounded_bottom_right, 12);
                                fusion_2_list(rounded_bottom_right, rounded_top_right, 12);
                                add_point_list(&inter_list, point_inter_haut.x, point_inter_haut.y);
                                add_point_list(&inter_list, point_inter_bas.x, point_inter_bas.y);
                                //fusion inter_list and rounded_top_right only from the middle
                                fusion_2_list(rounded_top_right, inter_list, 5);
                                ei_linked_point_t *rounded_down = rounded_bottom_left->next->next->next->next;
                                rounded_bottom_left->next->next->next->next = NULL;
                                free_linked_point_list(rounded_bottom_left);
                                free_linked_point_list(rounded_top_left);
                                return rounded_down;
                        }
                }
        }



}

void draw_button(ei_surface_t surface, ei_rect_t* rect_button, ei_color_t color, int border_width, int corner_radius, ei_relief_t relief, ei_rect_t* clipper)
{
        if (corner_radius != 0) {
                //case where no border => no relief
                if (border_width == 0) {
                        ei_linked_point_t* rounded0;
                        rounded0 = rounded_frame(rect_button, corner_radius, 0);
                        ei_draw_polygon(surface, rounded0, color, clipper);
                        free_linked_point_list(rounded0);
                }
                else {
                        //rect_button_on_screen = intersection between rect_button and clipper (what we see on screen)
			ei_rect_t rect_button_on_screen = inter_rect(rect_button, clipper);
                        ei_rect_t *rect_surface_with_border = malloc(sizeof(ei_rect_t));
                        //rect_surface_with_border = the "interior" of the button
                        rect_surface_with_border->size.height = rect_button->size.height - 2 * border_width;
                        rect_surface_with_border->size.width = rect_button->size.width - 2 * border_width;
                        rect_surface_with_border->top_left.x = rect_button->top_left.x + border_width;
                        rect_surface_with_border->top_left.y = rect_button->top_left.y + border_width;
                        if (relief == ei_relief_none) {
                                ei_linked_point_t *rounded_frame_button = rounded_frame(rect_button, corner_radius, 0);
                                ei_draw_polygon(surface, rounded_frame_button, dark_color(color),
                                                &rect_button_on_screen);
                                free_linked_point_list(rounded_frame_button);
                        }
                        if (relief == ei_relief_raised) {
                                //list of linked point for the part up and the part down of the button (to make the relief)
                                ei_linked_point_t *rounded_frame_up = rounded_frame(rect_button, corner_radius, 1);
                                ei_linked_point_t *rounded_frame_down = rounded_frame(rect_button, corner_radius, 2);
                                ei_draw_polygon(surface, rounded_frame_up, clear_color(color), &rect_button_on_screen);
                                ei_draw_polygon(surface, rounded_frame_down, dark_color(color), &rect_button_on_screen);
                                free_linked_point_list(rounded_frame_up);
                                free_linked_point_list(rounded_frame_down);
                        }
                        if (relief == ei_relief_sunken) {
                                ei_linked_point_t *rounded_frame_up = rounded_frame(rect_button, corner_radius, 1);
                                ei_linked_point_t *rounded_frame_down = rounded_frame(rect_button, corner_radius, 2);
                                ei_draw_polygon(surface, rounded_frame_up, dark_color(color), &rect_button_on_screen);
                                ei_draw_polygon(surface, rounded_frame_down, clear_color(color), &rect_button_on_screen);
                                free_linked_point_list(rounded_frame_up);
                                free_linked_point_list(rounded_frame_down);
                        }
                        //draw the "interior" of the button
                        ei_linked_point_t *rounded_frame_int = rounded_frame(rect_surface_with_border,
                                                                             corner_radius - border_width, 0);
                        ei_draw_polygon(surface, rounded_frame_int, color,
                                        &rect_button_on_screen);
                        free_linked_point_list(rounded_frame_int);
                        free(rect_surface_with_border);
                }
        }
        //case where radius corner is 0, we can use the draw of frame relief
        else{
                if (border_width>0) {
                        ei_rect_t* rect_button_on_screen = malloc(sizeof(ei_rect_t));
                        *rect_button_on_screen = inter_rect(clipper, rect_button);

                        if (relief == ei_relief_raised) {
                                draw_down_relief(rect_button, surface, color, EI_TRUE, rect_button_on_screen);

                                draw_up_relief(rect_button, surface, color, EI_TRUE, rect_button_on_screen);


                        }
                        if (relief == ei_relief_sunken) {
                                draw_up_relief(rect_button, surface, color, EI_FALSE, rect_button_on_screen);
                                draw_down_relief(rect_button, surface, color, EI_FALSE, rect_button_on_screen);

                        }
                        if (relief == ei_relief_none){
                                ei_color_t color_to_fill;
                                color_to_fill = dark_color(color);
                                ei_fill(surface, &color_to_fill, rect_button_on_screen);

                        }
                        free(rect_button_on_screen);

                }
                //draw the "interior" of the button, according to the border_width
                rect_button->top_left.x = rect_button->top_left.x + border_width;
                rect_button->top_left.y = rect_button->top_left.y + border_width;
                rect_button->size.width = rect_button->size.width - 2*border_width;
                rect_button->size.height = rect_button->size.height - 2*border_width;
		ei_rect_t rect_button_clipper = inter_rect(rect_button, clipper);
                ei_fill(surface, &color, &rect_button_clipper);
        }
}


ei_rect_t* draw_button_relief_up_down(ei_rect_t* rect_tot, int corner_radius, int border_width, int decalage_x, int decalage_y, int decalage_width, int decalage_height)
{
        ei_rect_t *rect_int = malloc(sizeof(ei_rect_t));
        int center_x = rect_tot->top_left.x + corner_radius;
        int center_y = rect_tot->top_left.y + corner_radius;
        //take into account of the decalage to determinate rect_int (rectangle of the "interior" of the button)
        rect_int->top_left.x = (int) (center_x + (corner_radius - border_width) *
                                                 cos(-5 * M_PI / 4)) + decalage_x;
        rect_int->top_left.y = (int) (center_y - (corner_radius - border_width) *
                                                 sin(-5 * M_PI / 4)) + decalage_y;
        rect_int->size.width =
                (int) (rect_tot->top_left.x + rect_tot->size.width - corner_radius +
                       (corner_radius - border_width) * cos(-7 * M_PI / 4)) -
                (int) (center_x + (corner_radius - border_width) * cos(-5 * M_PI / 4)) - decalage_width;
        rect_int->size.height =
                (int) (rect_tot->top_left.y + rect_tot->size.height - corner_radius -
                       (corner_radius - border_width) * sin(-3 * M_PI / 4)) -
                (int) (center_y - (corner_radius - border_width) * sin(-5 * M_PI / 4)) - decalage_height;

        return rect_int;

}
