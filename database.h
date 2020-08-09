#pragma once

#include "linked_list.h"

#define USER_DATABASE "user_database.txt"
#define CHAT_DATABASE "chat_database.txt"

//populate user list from text file
ListHead *database_get_users(void);

//populate chat list for an user from txt file
ListHead *database_get_chats(char *username);

//add new user to databse
void database_add_user(ListItem *new_user);
