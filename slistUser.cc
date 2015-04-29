#include <stdio.h>
#include <stdlib.h>
//#include "slistUser.h"
void
sllist_initUser(SLListUser * list)
{
	list->head = NULL;
}

// Add int value to the end of list. The values may be repeated.
void
sllist_add_begUser( SLListUser *list, char * name, char* password)  {
	SLEntryUser* n = (SLEntryUser *)malloc(sizeof(SLEntryUser));
	n-> name= strdup(name);
	n-> password = strdup(password);

	n->next = list->head;
	list->head = n;
}
void 
sllist_removeUser(SLListUser *list,  char * name) {
	SLEntryUser * e;
	SLEntryUser * prev = NULL;
	e = list->head;
	while(e != NULL){
		if(strcmp(e->name, name)==0){
			if(e == list->head){
				list->head = e->next;
				free(e);
			}
			else{
				prev->next = e->next;
				free(e);
			}
		}
		prev = e;
		e = e->next;
	}
}
void
sllist_sort(SLListUser *list)  {
		SLEntryUser * e;
		int swapped = 0;
 		do
		{
			char * temp;
			char * tempPassword;
	    	swapped = 0;
	    	e = list->head;
	    	while (e->next != NULL)
	    	{
            	if (strcmp(e->name, e->next->name) > 0)
	        	{
	           		temp = strdup (e -> name);
	           		tempPassword = strdup(e->password);
			    	e->name = strdup(e->next ->name);
			    	e->password = strdup(e->next ->password);
			    	e->next->name = strdup(temp);
			    	e->next -> password = strdup(tempPassword);
		        	swapped = 1;
				}
		    	e = e->next;
			}
		}
		while (swapped);

}

