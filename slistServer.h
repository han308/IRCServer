#include "slistUser.h"
#include "slistUser.cc"
#include "slistMessage.h"
#include "slistMessage.cc"
//#include "slistServer.cc"
struct SLEntryServer {
  char * roomName;
  SLListMessage * messages;
  SLListUser * users;

  struct SLEntryServer * next;
};

typedef struct SLEntryServer SLEntryServer;

struct SLListServer {
  SLEntryServer * head;
};

typedef struct SLListServer SLListServer;

void sllist_initServer(SLListServer * list);
void sllist_add_begServer( SLListServer *list, char * roomName);
void sllist_removeServer(SLListServer *list, char * roomName);
void sllist_sortRooms( SLListServer *list);

