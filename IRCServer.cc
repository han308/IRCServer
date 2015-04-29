
 char * usage =
"                                                               \n"
"IRCServer:                                                   \n"
"                                                               \n"
"Simple server program used to communicate multiple users       \n"
"                                                               \n"
"To use it in one window type:                                  \n"
"                                                               \n"
"   IRCServer <port>                                          \n"
"                                                               \n"
"Where 1024 < port < 65536.                                     \n"
"                                                               \n"
"In another window type:                                        \n"
"                                                               \n"
"   telnet <host> <port>                                        \n"
"                                                               \n"
"where <host> is the name of the machine where talk-server      \n"
"is running. <port> is the port number you used when you run    \n"
"daytime-server.                                                \n"
"                                                               \n";

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>
using namespace std;

#include "IRCServer.h"
#include "slistServer.h"
#include "slistServer.cc"

SLListServer rooms;
SLListUser allUsers;

int QueueLength = 5;

int
IRCServer::open_server_socket(int port) {

	// Set the IP address and port for this server
	struct sockaddr_in serverIPAddress; 
	memset( &serverIPAddress, 0, sizeof(serverIPAddress) );
	serverIPAddress.sin_family = AF_INET;
	serverIPAddress.sin_addr.s_addr = INADDR_ANY;
	serverIPAddress.sin_port = htons((u_short) port);
  
	// Allocate a socket
	int masterSocket =  socket(PF_INET, SOCK_STREAM, 0);
	if ( masterSocket < 0) {
		perror("socket");
		exit( -1 );
	}

	// Set socket options to reuse port. Otherwise we will
	// have to wait about 2 minutes before reusing the sae port number
	int optval = 1; 
	int err = setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, 
			     (char *) &optval, sizeof( int ) );
	
	// Bind the socket to the IP address and port
	int error = bind( masterSocket,
			  (struct sockaddr *)&serverIPAddress,
			  sizeof(serverIPAddress) );
	if ( error ) {
		perror("bind");
		exit( -1 );
	}
	
	// Put socket in listening mode and set the 
	// size of the queue of unprocessed connections
	error = listen( masterSocket, QueueLength);
	if ( error ) {
		perror("listen");
		exit( -1 );
	}

	return masterSocket;
}

void
IRCServer::runServer(int port)
{
	int masterSocket = open_server_socket(port);

	initialize();
	
	while ( 1 ) { 
		
		// Accept incoming connections
		struct sockaddr_in clientIPAddress;
		int alen = sizeof( clientIPAddress );
		int slaveSocket = accept( masterSocket,
					  (struct sockaddr *)&clientIPAddress,
					  (socklen_t*)&alen);
		
		if ( slaveSocket < 0 ) {
			perror( "accept" );
			exit( -1 );
		}
		 
		// Process request.
		processRequest( slaveSocket );		
	}
}

int
main( int argc, char ** argv )
{
	// Print usage if not enough arguments
	if ( argc < 2 ) {
		fprintf( stderr, "%s", usage );
		exit( -1 );
	}
	
	// Get the port from the arguments
	int port = atoi( argv[1] );

	IRCServer ircServer;

	// It will never return
	ircServer.runServer(port);
	
}
void
IRCServer::processRequest( int fd )
{
	// Buffer used to store the comand received from the client
	 int MaxCommandLine = 1024;
	char commandLine[ MaxCommandLine + 1 ];
	int commandLineLength = 0;
	int n;
	
	// Currently character read
	unsigned char prevChar = 0;
	unsigned char newChar = 0;
	
	//
	// The client should send COMMAND-LINE\n
	// Read the name of the client character by character until a
	// \n is found.
	//

	// Read character by character until a \n is found or the command string is full.
	while ( commandLineLength < MaxCommandLine &&
		read( fd, &newChar, 1) > 0 ) {

		if (newChar == '\n' && prevChar == '\r') {
			break;
		}
		
		commandLine[ commandLineLength ] = newChar;
		commandLineLength++;

		prevChar = newChar;
	}
	
	// Add null character at the end of the string
	// Eliminate last \r
	commandLineLength--;
        commandLine[ commandLineLength ] = 0;

	printf("RECEIVED: %s\n", commandLine);

	char * c = (char*)malloc(sizeof(char)*50);
	char * u = (char*)malloc(sizeof(char)*50);
	char * p = (char*)malloc(sizeof(char)*50);
	char * a = (char*)malloc(sizeof(char)*50);
	char * m = (char*)malloc(sizeof(char)*1024);
	//char c[50];
	//char u[50];
	//char p[50];
	//char a[50];
	//char m[200];
	int k = 0;
	//seperate inputs    {
	char * line = strtok (commandLine, " ");

	while (line != NULL){
		
		if(k == 0){
			c = strdup(line);
			
		}
		else if(k == 1){
			u = strdup(line);
		}
		else if(k == 2){
			p = strdup(line);
		}
		else if (k == 3){
			a = strdup(line);
		}
		else if (k == 4){
			m = strdup(line);
		}
		else{
			strcat(m," ");
			strcat(m, strdup(line));
		}
		line = strtok (NULL, " ");
		k++;
  	}

	printf("RECEIVED: %s\n", c);
	printf("RECEIVED: %s\n", u);
	printf("RECEIVED: %s\n", p);
	printf("RECEIVED: %s\n", a);
	printf("RECEIVED: %s\n", m);
	 char * command = c;
	 char * user = u;
	 char * password = p;
	 char * args = a;
	 char * message = m;

	printf("command=%s\n", command);
	printf("user=%s\n", user);
	printf( "password=%s\n", password );
	printf("args=%s\n", args);
	printf("message=%s\n", message);

	if (!strcmp(command, "ADD-USER")) {
		addUser(fd, user, password, args);
	}
	else if (!strcmp(command, "CREATE-ROOM")) {
		createRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "ENTER-ROOM")) {
		enterRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "LEAVE-ROOM")) {
		leaveRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "SEND-MESSAGE")) {
		sendMessage(fd, user, password, args, message);
	}
	else if (!strcmp(command, "GET-MESSAGES")) {
		getMessages(fd, user, password, args, message);
	}
	else if (!strcmp(command, "GET-USERS-IN-ROOM")) {
		getUsersInRoom(fd, user, password, args);
	}
	else if (!strcmp(command, "LIST-ROOMS")) {
		listRooms(fd, user, password, args);
	}
	else if (!strcmp(command, "GET-ALL-USERS")) {
		getAllUsers(fd, user, password, args);
	}
	else {
		 const char * msg =  "UNKNOWN COMMAND\r\n";
		write(fd, msg, strlen(msg));
	}

	// Send OK answer
	// const char * msg =  "OK\n";
	//write(fd, msg, strlen(msg));

	close(fd);	
}

void
IRCServer::initialize()
{
	// Open password file

	// Initialize users in room
	sllist_initServer(&rooms);
	sllist_initUser(&allUsers);
	// Initalize message list

}

bool
IRCServer::checkPassword(int fd,  char * user,  char * password) {
	// Here check the password
	//char * u = strdup(user);
	//char * p = strdup(password);
	SLListUser * list = &allUsers;
	SLEntryUser* e = list->head;
	while (e != NULL) {
		if (strcmp(e->name, user) == 0){
			if (strcmp(e->password, password)==0){
				return 1;
			}
		}
		e = e->next;
	}
	return 0;
}

void
IRCServer::addUser(int fd,  char * user,  char * password,  char * args)
{
	// Here add a new user. For now always return OK.
	//char * u = strdup(user);
	//char * p = strdup(password);
	SLListUser * list = &allUsers;
	SLEntryUser* e = list->head;
	while (e != NULL) {
		if (strcmp(e->name, user) == 0){
			 const char * msg =  "ERROR (User exists)\r\n";
			write(fd, msg, strlen(msg));
			close(fd);
			return;
		}
		e = e->next;
	}
	sllist_add_begUser(list, user, password);
	 const char * msg =  "OK\r\n";
	write(fd, msg, strlen(msg));
	
	ofstream pw;
  	pw.open ("password.txt");
 	pw << user;
 	pw << "\n";
 	pw << password;
 	pw << "\n";
  	pw.close();
  	close(fd);
	return;		
}
void
IRCServer::createRoom(int fd,  char * user,  char * password,  char * args)
{
	//char * u = strdup(user);
	//char * p = strdup(password); 
	//char * rN = strdup (args);
	if(checkPassword(fd, user, password)){
		SLListServer * list = &rooms;
		sllist_add_begServer(list, args);
		 const char * msg =  "OK\r\n";
		write(fd, msg, strlen(msg));
	}
	else{
		 const char * msg =  "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
		return;		
	}
	close(fd);
}

void
IRCServer::enterRoom(int fd,  char * user,  char * password,  char * args)
{
	//char * u = strdup(user);
	//char * p = strdup(password);
	//char * rN = strdup(args);
	if(checkPassword(fd, user, password)){
		SLListServer * list = &rooms;
		SLEntryServer* e = list->head;
		//sllist_addUser(list, u, rN, p);
		while (e != NULL) {
			if (strcmp(e->roomName, args) == 0){
				SLEntryUser* a = e->users->head;
				while (a != NULL) {
					if (strcmp(a->name, user) == 0){
										 const char * msg =  "OK\r\n";
				write(fd, msg, strlen(msg));
						return;
					}
					a = a->next;
				}
				sllist_add_begUser(e->users, user, password);
				 const char * msg =  "OK\r\n";
				write(fd, msg, strlen(msg));
				return;
			}
		e = e->next;
	}
		 const char * msg =  "ERROR (No room)\r\n";
		write(fd, msg, strlen(msg));
	}
	else{
		 const char * msg =  "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
		return;		
	}
	close(fd);
}

void
IRCServer::leaveRoom(int fd,  char * user,  char * password,  char * args)
{
	//char * u = strdup(user);
	//char * p = strdup(password);
	//char * rN = strdup(args);
	if(checkPassword(fd, user, password)){
		SLListServer * list = &rooms;
		SLEntryServer* e = list->head;
		//sllist_removeUserRoom(list, u, rN);
		while (e != NULL) {
			if (strcmp(e->roomName, args) == 0){
				SLEntryUser * a = e->users->head;
				while(a != NULL){
					if(strcmp(a->name, user) == 0){
						sllist_removeUser(e->users, user);
						 const char * msg =  "OK\r\n";
						write(fd, msg, strlen(msg));
						close(fd);
						return;
					}
					a = a->next;
				}
				 const char * msg =  "ERROR (No user in room)\r\n";
				write(fd, msg, strlen(msg));
				close(fd);
				return;
			}
			e = e->next;
		}

	}
	else{
		 const char * msg =  "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
		return;		
	}
	close(fd);
}

void
IRCServer::sendMessage(int fd,  char * user,  char * password,  char * args,  char * message)
{
	//char * u = strdup(user);
	//char * p = strdup(password);
	//char * rN = strdup(args);
	//char * m = strdup(message);
	int numMessage = 0;
	if(checkPassword(fd, user, password)){
		SLListServer * list = &rooms;
		SLEntryServer* e = list->head;
		while (e != NULL) {
			if (strcmp(e->roomName, args) == 0){
				SLEntryUser * a = e->users->head;
				while(a != NULL){
					if (strcmp(a->name, user) == 0){
						SLEntryMessage * b = e->messages->head;
						while(b != NULL){
							numMessage++;
							b=b->next;
						}
						if(numMessage == 100){
							sllist_removeMessage(e->messages, message);
							sllist_changeMsgNum(e->messages);
							numMessage--;
							char * number = (char*)malloc(sizeof(char)*10);
							sprintf(number, "%d", numMessage);
							sllist_add_endMessage(e->messages, user, message, number);
							 const char * msg =  "OK\r\n";
							write(fd, msg, strlen(msg));
							close(fd);
							return;
						}
						else{
							char * number = (char*)malloc(sizeof(char)*10);
							sprintf(number, "%d", numMessage);
							sllist_add_endMessage(e->messages, user, message, number);
							 const char * msg =  "OK\r\n";
							write(fd, msg, strlen(msg));
							close(fd);
							return;
						}
					}
					a = a->next;
				}
				 const char * msg =  "ERROR (user not in room)\r\n";
				write(fd, msg, strlen(msg));
				close(fd);
				return;
			}
			e = e->next;
		}

	}
	else{
		 const char * msg =  "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
			close(fd);
			return;
	}
}


void
IRCServer::getMessages(int fd,  char * user,  char * password,  char * args,  char * message)
{
	//char * u = strdup(user);
	//char * p = strdup(password);
	//char * mN = strdup(args);
	//char * rN = strdup(message);
	int lastMessageNum = 0;
	int msgNum = *args - '0';

	if(checkPassword(fd, user, password)){
		SLListServer * list = &rooms;
		SLEntryServer* e = list->head;
		while (e != NULL) {
			if (strcmp(e->roomName, message) == 0){
				SLEntryUser * a = e->users->head;
				while(a != NULL){
					if (strcmp(a->name, user) == 0){
						SLEntryMessage * b = e->messages->head;
						while(b != NULL){ 
							lastMessageNum++;
							b=b->next;
						}
						unsigned int k = 0;
						b = e->messages->head;
						if(msgNum >= lastMessageNum){
							 const char * msg =  "NO-NEW-MESSAGES\r\n";
							write(fd, msg, strlen(msg));
							close(fd);
							return;
						}
						else{
							while(b != NULL){
								if(msgNum <= k){
									char * printMessage = (char*)malloc(sizeof(char)*100);
									//char * number = (char*)malloc(sizeof(char)*100);
									//sprintf(number, "%d", (b->msgNum));
									printMessage = strdup(b->msgNum);
									strcat(printMessage," ");
									strcat(printMessage, strdup(b->username));
									strcat(printMessage," "); 
									strcat(printMessage, strdup(b->message));
									strcat(printMessage,"\r\n");
									write(fd, printMessage, strlen(printMessage));
									//write(fd, (char*)*(b->msgNum), strlen((char*)*(b->msgNum)));
									//write(fd, printMessage, strlen(printMessage));
									//write(fd, b->msgNum, strlen(b->msgNum));
									//write(fd, b->username, strlen(user));
									//write(fd, b->message, strlen(message));
								}
								b=b->next;
								k++;
							}
							write(fd, "\r\n", strlen("\r\n"));
							close(fd);
							return;
						}

					}
					a = a->next;
				}
				 const char * msg =  "ERROR (User not in room)\r\n";
				write(fd, msg, strlen(msg));
				close(fd);
				return;
			}
			e = e->next;
		}

	}
	else{
		 const char * msg =  "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
		return;
	}
	close(fd);

}

void
IRCServer::getUsersInRoom(int fd,  char * user,  char * password,  char * args)
{
	//char * u = strdup(user);
	//char * p = strdup(password);
	//char * rN = strdup(args);
	if(checkPassword(fd, user, password)){
		SLListServer * list = &rooms;
		if(list->head != NULL){
			SLEntryServer* e = list->head;
			while (e != NULL) {
				if (strcmp(e->roomName, args) == 0){
					if(e->users->head !=NULL){
						sllist_sort(e->users);
						SLEntryUser * a = e->users->head;
						while (a != NULL){
							write(fd, a->name, strlen(a->name));
							write(fd, "\r\n", strlen("\r\n"));
							a = a->next;
						}
						write(fd, "\r\n", strlen("\r\n"));
						close(fd);
						return;
					}
					write(fd, "\r\n", strlen("\r\n"));
				}
				e = e->next; 
			}
		}
	}
	else{
		 const char * msg =  "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
		close(fd);
		return;
	}
	close(fd);
}

void
IRCServer::getAllUsers(int fd,  char * user,  char * password,  char * args)
{
	//char * u = strdup(user);
	//char * p = strdup(password);
	if(checkPassword(fd, user, password)){
		SLListUser * list = &allUsers;
	 	sllist_sort(list);
		SLEntryUser * e = list->head;
		while(e != NULL){
			write(fd, e->name, strlen(e->name));
			write(fd, "\r\n", strlen("\r\n"));
			e = e->next;
		}
		write(fd, "\r\n", strlen("\r\n"));
	}
	else{
		 const char * msg =  "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
		return;
	}
	close(fd);	

}
void
IRCServer::listRooms(int fd, char * user,  char * password,  char * args)
{
	//char * u = strdup(user);
	//har * p = strdup(password);
	if(checkPassword(fd, user, password)){
		SLListServer * list = &rooms;
		sllist_sortRooms(list);
		SLEntryServer* e = list->head;
		while(e != NULL){
			write(fd, e->roomName, strlen(e->roomName));
			write(fd, "\r\n", strlen("\r\n"));
			//write(fd, "\n", 2);
			e = e->next;
		}
		write(fd, "\r\n", strlen("\r\n"));
	}
	else{
		 const char * msg =  "ERROR (Wrong password)\r\n";
		write(fd, msg, strlen(msg));
		return;
	}
	close(fd);	

}

