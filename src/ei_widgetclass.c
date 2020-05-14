/******************************************************************************
* File:             ei_widgetclass.c
*
* Author:           Robin BERTIN (Nunwan)
* Created:          05/13/20
* Description:      
*****************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "ei_widgetclass.h"

// save the head of the widgetclass head
ei_widgetclass_t sentinel = {"sentinel", NULL, NULL, NULL, NULL, NULL, NULL};

/*
 * @brief	Update the pointer of widgetclass to the next one
 *
 * @param	widgetclass_pt pointer to a widget class
 */
void widgetclass_suivant(ei_widgetclass_t** widgetclass_pt)
{
	*widgetclass_pt = (*widgetclass_pt)->next;
}


void ei_widgetclass_register(ei_widgetclass_t* widgetclass)
{
	ei_widgetclass_t *widgetclass_list = &sentinel;
	while (widgetclass_list->next != NULL) {
		// Check if the class is not register twice.
		assert(strcmp(widgetclass->name, widgetclass_list->name) != 0);
		widgetclass_suivant(&widgetclass_list);
	}
	widgetclass_list->next = widgetclass;
}


ei_widgetclass_t* ei_widgetclass_from_name(ei_widgetclass_name_t name)
{
	for (ei_widgetclass_t* widgetc = &sentinel; widgetc != NULL; widgetclass_suivant(&widgetc)) {
		printf("%s, %i", widgetc->name, strcmp(name, widgetc->name));
		if (strcmp(name, widgetc->name) == 0) {
			return widgetc;
		}
	}
}

