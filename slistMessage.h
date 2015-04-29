struct SLEntryMessage {
  char * username;
  char * message;
  char * msgNum;

  struct SLEntryMessage * next;
};

typedef struct SLEntryMessage SLEntryMessage;

struct SLListMessage{
  SLEntryMessage * head;
};

typedef struct SLListServer SLListServer;

void sllist_initMessage(SLListMessage * list);
void sllist_add_endMessage(SLListMessage*list, char * user,char * message, char * msgNum);
void sllist_removeMessage(SLListMessage *list, char * message);
void sllist_changeMsgNum(SLListMessage *list);

