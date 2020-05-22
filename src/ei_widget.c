/******************************************************************************
* File:             ei_widget.c
*
* Author:           Robin BERTIN (Nunwan)
* Created:          05/12/20
* Description:      Implementation for widgets management.
*****************************************************************************/
#include <stdlib.h>

#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "traverse_tools.h"
#include "ei_application.h"




void update_widget_list(ei_widget_t* child, ei_widget_t* parent)
{
	child->parent = parent;
	if (parent != NULL) {
		if (parent->children_head == NULL) {
			parent->children_head = child;
		}
		else {
			parent->children_tail->next_sibling = child;
		}
		parent->children_tail = child;
	}
}


void ei_widget_destroy(ei_widget_t* widget)
{
	if (widget->parent != NULL) {
		ei_widget_t* parent_child = widget->parent->children_head;
		if (parent_child == widget) {
                widget->parent->children_head = widget->next_sibling;
			if (widget->parent->children_tail == widget) {
				widget->parent->children_tail = NULL;
			}

		}
		else {
			while (parent_child->next_sibling != widget) {
				parent_child = parent_child->next_sibling;
			}
			parent_child->next_sibling = widget->next_sibling;
			if (parent_child->next_sibling == NULL) {
				widget->parent->children_tail = parent_child;
			}
		}
	}
	destroy_widgets(widget);
	if (widget->destructor != NULL) {
		widget->destructor(widget);
	}
	ei_app_invalidate_rect(&(widget->screen_location));
	widget->wclass->releasefunc(widget);
	free(widget->geom_params);
	free(widget);
}


ei_widget_t*	ei_widget_create	(ei_widgetclass_name_t	class_name,
					 ei_widget_t*		parent,
					 void*			user_data,
					 ei_widget_destructor_t	destructor)
{
	ei_widgetclass_t* widgetclass = ei_widgetclass_from_name(class_name);
	if (widgetclass != NULL) {
		ei_widget_t* newWidget = (ei_widget_t *)widgetclass->allocfunc();
		newWidget->wclass = widgetclass;
		newWidget->user_data = user_data;
		newWidget->destructor = destructor;
                newWidget->wclass->setdefaultsfunc(newWidget);
                update_widget_list(newWidget, parent);
                return newWidget;
	}
}
