#pragma once

#include "user_list.h"

//client-side login routine
int client_login(int socket_desc, char *username[MAX_LENGTH_USERNAME]);

//server-side login routine
int server_login(int socket_desc, ListHead *userHead, ListHead** chatHead);