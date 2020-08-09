#pragma once

#include "common.h"
#include "linked_list.h"
#include "user_list.h"

//generic chat structure
typedef struct
{
    ListItem listItem;
    char with[MAX_LENGTH_USERNAME];
} ChatListItem;

//head of chat list
typedef struct {
    ListHead listHead;
    char username[MAX_LENGTH_USERNAME];
} ChatListHead;

//print chat list
void ChatList_print(ListHead *head);
