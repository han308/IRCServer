#include <stdio.h>
#include <stdlib.h>
//#include "slistUser.h"
void
sllist_initMessage(SLListMessage * list)
{
	list->head = NULL;
}

// Add int value to the end of list. The values may be repeated.
void
sllist_add_endMessage(SLListMessage *list, char * user, char* message, char * msgNum){
	SLEntryMessage* n = (SLEntryMessage *)malloc(sizeof(SLEntryMessage));
	n-> username= user;
	n-> message = message;
	n-> msgNum = msgNum;

	SLEntryMessage* e;
	e = list->head;
	if(e == NULL){
		list->head = n;
	}
	if (e != NULL){
		while(e->next != NULL){
			e = e->next;
		}
		e->next = n;
	}
}
// Remove first occurrence of value in the list. It returns 1 if value is found or 0 otherwise.
void
sllist_removeMessage(SLListMessage*list, char * message) {
	SLEntryMessage * e;
	SLEntryMessage* prev = NULL;
	e = list->head;
	while(e != NULL){
		if(strcmp(e->message, message)==0){
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
sllist_changeMsgNum(SLListMessage *list)  {
	SLEntryMessage * e = list->head;
	int msgNum = 0;
	while(e != NULL){
		char  number [10];
		sprintf(number, "%d", msgNum);
		e->msgNum = number;
		msgNum++;
		e = e->next;
	}
}

