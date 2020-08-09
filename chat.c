#include <arpa/inet.h>  // htons() and inet_addr()
#include <errno.h>
#include <netinet/in.h> // struct sockaddr_in
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "chat_list.h"
#include "common.h"
#include "database.h"

//client-side chat routine
void client_chat(int socket_desc) {
    if (DEBUG) fprintf(stderr, "CHAT ROUTINE STARTED\n");
    printf("\nRetrieving chats...\n\n");
}

//server-side chat routine
void server_chat(int socket_desc, ListHead* chatHead)
{
    if (DEBUG) fprintf(stderr, "CHAT ROUTINE STARTED\n");

    if (DEBUG) ChatList_print(chatHead);
}