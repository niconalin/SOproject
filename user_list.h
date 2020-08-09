#pragma once

#include "linked_list.h"

// user list item
typedef struct
{
    ListItem listItem;
    char username[MAX_LENGTH_USERNAME];
    char password[MAX_LENGTH_PASSWORD];
    ListHead *chats;
} UserListItem;

//print a user list
void UserList_print(ListHead *head);

//create new user account
UserListItem *UserList_create_user(ListHead *head,
    char username[MAX_LENGTH_USERNAME],
    char password[MAX_LENGTH_PASSWORD]);

//find user by username
UserListItem *UserList_find(ListHead *head, char username[64]);

//free memory allocated for the list
void UserList_free(ListHead *head);