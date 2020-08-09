#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "linked_list.h"
#include "user_list.h"

void UserList_print(ListHead *head)
{
    ListItem *aux = head->first;
    printf("[\n");
    while (aux)
    {
        UserListItem *userListItem = (UserListItem *)aux;
        printf("us: %s, pw: %s\n", userListItem->username, userListItem->password);
        aux = aux->next;
    }
    printf("]\n");
}

UserListItem *UserList_create_user(ListHead *head,
    char username[MAX_LENGTH_USERNAME],
    char password[MAX_LENGTH_PASSWORD])
{
    UserListItem *userListItem = (UserListItem *)malloc(sizeof(UserListItem));
    if (!userListItem)
        handle_error("Out of memory!");
    userListItem->listItem.prev = 0;
    userListItem->listItem.prev = 0;
    strcpy(userListItem->username, username);
    strcpy(userListItem->password, password);
    if (!List_insert(head, head->last, (ListItem *)userListItem))
        handle_error("Cannot create new user");
    return userListItem;
}

UserListItem *UserList_find(ListHead *head, char username[MAX_LENGTH_USERNAME])
{
    ListItem *aux = head->first;
    while (aux)
    {
        UserListItem *userListItem = (UserListItem *)aux;
        if (strcmp(userListItem->username, username) == 0)
        {
            return userListItem;
        }
        aux = aux->next;
    }
    return NULL;
}

void UserList_free(ListHead *head)
{
    ListItem *item = head->first;
    ListItem *next = NULL;
    while (item)
    {
        next = item->next;
        free(item);
        item = next;
        next = NULL;
    }
    free(head);
}