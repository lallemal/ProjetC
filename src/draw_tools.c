/******************************************************************************
* File:             draw_tools.c
*
* Author:           Robin BERTIN (Nunwan)
* Created:          05/15/20
* Description:
*****************************************************************************/
#include <stdlib.h>
#include <string.h>


#include "ei_widgetclass.h"
#include "ei_widget.h"
#include "ei_draw.h"
#include "math.h"
#include "draw_tools.h"



#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

ei_point_t* anchor_point(ei_surface_t surface, ei_rect_t* rect, ei_anchor_t anchor, int width_text, int height_text)
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
        if (anchor == ei_anc_center){
                point->x=min((x_rect+x_rect+width)/2, x_rect+width-width_text)-width_text/2;
                point->y=min((y_rect+y_rect+height)/2, y_rect+height-height_text)-height_text/2;
        }
        if(anchor == ei_anc_north){
                point->x=min((x_rect+x_rect+width)/2, x_rect+width-width_text)-width_text/2;
                point->y=y_rect;
        }
        if(anchor == ei_anc_northeast){
                point->x=x_rect+width-width_text;
                point->y=y_rect;
        }
        if(anchor == ei_anc_east){
                point->x=x_rect+width-width_text;
                point->y=min((y_rect+y_rect+height)/2, y_rect+height-height_text)-height_text/2;
        }
        if(anchor == ei_anc_southeast){
                point->x=x_rect+width-width_text;
                point->y=y_rect+height-height_text;
        }
        if(anchor == ei_anc_south){
                point->x=min((x_rect+x_rect+width)/2, x_rect+width-width_text)-width_text/2;
                point->y=y_rect+height-height_text;
        }
        if(anchor == ei_anc_southwest){
                point->x=x_rect;
                point->y=y_rect+height-height_text;
        }
        if(anchor == ei_anc_west){
                point->x=x_rect;
                point->y=min((y_rect+y_rect+height)/2, y_rect+height-height_text)-height_text/2;
        }
        if(anchor == ei_anc_northwest){
                point->x=x_rect;
                point->y=y_rect;
        }
        return point;
}

ei_color_t dark_color(ei_color_t color)
{
        ei_color_t color_clearer;
        if (color.blue>20) {
                color_clearer.blue = color.blue - 20;
        }
        else{
                color_clearer.blue = color.blue;
        }
        if (color.green>20) {
                color_clearer.green = color.green - 20;
        }
        else{
                color_clearer.green = color.green;
        }
        if (color.red>20) {
                color_clearer.red = color.red - 20;
        }
        else{
                color_clearer.red = color.red;
        }
        color_clearer.alpha=color.alpha;
        return color_clearer;
}


ei_color_t clear_color(ei_color_t color)
{
        ei_color_t color_darker;
        if (color.blue<235) {
                color_darker.blue = color.blue + 20;
        }
        else{
                color_darker.blue = color.blue;
        }
        if (color.green<235) {
                color_darker.green = color.green + 20;
        }
        else{
                color_darker.green = color.green;
        }
        if (color.red<235) {
                color_darker.red = color.red + 20;
        }
        else{
                color_darker.red = color.red;
        }
        color_darker.alpha=color.alpha;
        return color_darker;
}


void draw_up_relief(ei_rect_t* rect_to_fill, ei_surface_t surface, ei_color_t color, ei_bool_t clear_up)
{

        //up
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
        fourth_point_up.y = rect_to_fill->top_left.y + min(rect_to_fill->size.height/2, rect_to_fill->size.width/2);
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
        if (clear_up==EI_TRUE) {
                ei_draw_polygon(surface, list_point_up1, clear_color(color), rect_to_fill);
        }
        if (clear_up==EI_FALSE){
                ei_draw_polygon(surface, list_point_up1, dark_color(color), rect_to_fill);
        }
        free(list_point_up1);
        free(list_point_up2);
        free(list_point_up3);
        free(list_point_up4);
        free(list_point_up5);


}

void draw_down_relief(ei_rect_t* rect_to_fill, ei_surface_t surface, ei_color_t color, ei_bool_t clear_up)
{
        //down
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
        fourth_point_down.y = rect_to_fill->top_left.x - min(rect_to_fill->size.height/2, rect_to_fill->size.width/2);
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
        if (clear_up==EI_TRUE) {
                ei_draw_polygon(surface, list_point_down1, dark_color(color), rect_to_fill);
        }
        if (clear_up==EI_FALSE){
                ei_draw_polygon(surface, list_point_down1, clear_color(color), rect_to_fill);
        }
        free(list_point_down1);
        free(list_point_down2);
        free(list_point_down3);
        free(list_point_down4);
        free(list_point_down5);


}

ei_linked_point_t* arc_point(ei_point_t center, int radius, int corner_begin, int corner_end)
{
        int x_center = center.x;
        int y_center = center.y;
        ei_linked_point_t* list_point[10];
        for (int i=0; i<10; i++){
                if (i<9){
                        list_point[i]->point.x = x_center + radius*cos(corner_begin + i*(1/10)*(corner_end - corner_begin));
                        list_point[i]->point.y = y_center + radius*sin(corner_begin + i*(1/10)*(corner_end - corner_begin));
                        list_point[i]->next = list_point[i+1];
                }
                else{
                        list_point[i]->point.x = x_center + radius*cos(corner_begin + i*(1/10)*(corner_end - corner_begin));
                        list_point[i]->point.y = y_center + radius*sin(corner_begin + i*(1/10)*(corner_end - corner_begin));
                        list_point[i]->next = NULL;
                }
        }
        return list_point[10];

}

ei_linked_point_t* rounded_frame(ei_rect_t* rect, int radius)
{
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
        ei_linked_point_t* rounded_top_left = arc_point(center_top_left, radius, (M_PI)/2, M_PI);
        ei_linked_point_t* rounded_top_right = arc_point(center_top_right, radius,0,  (M_PI)/2 );
        ei_linked_point_t* rounded_bottom_left = arc_point(center_bottom_left, radius, M_PI, (3*M_PI)/2);
        ei_linked_point_t* rounded_bottom_right = arc_point(center_bottom_right, radius, (3*M_PI)/2, 2*M_PI);
        rounded_top_left[10].next = &rounded_top_right[0];
        rounded_top_right[10].next = &rounded_bottom_right[0];
        rounded_bottom_right[10].next = &rounded_bottom_left[0];
        return rounded_top_left;

}