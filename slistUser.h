struct SLEntryUser {
  char * name;
  char * password;

  struct SLEntryUser * next;
};

typedef struct SLEntryUser SLEntryUser;

struct SLListUser {
  SLEntryUser * head;
};

typedef struct SLListUser SLListUser;

void sllist_initUser(SLListUser * list);
void sllist_add_begUser( SLListUser *list,  char * name, char* password);
void sllist_removeUser(SLListUser *list, char * name);
void sllist_sort( SLListUser *list);
