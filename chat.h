#pragma once

#include "common.h"
#include "linked_list.h"

//chat core for client
void client_chat(int socket_desc, char* username);

//chat core for server
int server_chat(int socket_desc, ListHead* chatHead);