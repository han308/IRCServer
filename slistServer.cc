
#include <stdio.h>
#include <stdlib.h>
//#include "slistServer.h"
void
sllist_initServer(SLListServer * list)
{
	list->head = NULL;
}

// Add int value to the end of list. The values may be repeated.
void
sllist_add_begServer( SLListServer *list, char * roomName)  {
	SLEntryServer* n = (SLEntryServer *)malloc(sizeof(SLEntryServer));
	n-> roomName = strdup(roomName);
	n->users = (SLListUser *)malloc(sizeof(SLListUser));
	n->messages = (SLListMessage *)malloc(sizeof(SLListMessage));
	sllist_initUser((n->users));
	sllist_initMessage((n->messages));
	n->next = list->head;
	list->head = n;
}
// Remove first occurrence of value in the list. It returns 1 if value is found or 0 otherwise.
void
sllist_removeServer(SLListServer *list, char * roomName) {
	SLEntryServer * e;
	SLEntryServer * prev = NULL;
	e = list->head;
	while(e != NULL){
		if(e->roomName == roomName){
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
sllist_sortRooms(SLListServer *list)  {
		SLEntryServer * e;
		int swapped = 0;
 		do
		{
			char * temp;
	    	swapped = 0;
	    	e = list->head;
	    	while (e->next != NULL)
	    	{
            	if (strcmp(e->roomName, e->next->roomName) > 0)
	        	{
	           		temp = strdup (e -> roomName);
			    	e->roomName = strdup(e->next ->roomName);
			    	e->next->roomName = strdup(temp);
		        	swapped = 1;
				}
		    	e = e->next;
			}
		}
		while (swapped);

}


