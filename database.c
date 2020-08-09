#include <errno.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chat_list.h"
#include "common.h"
#include "database.h"
#include "linked_list.h"
#include "user_list.h"

ListHead *database_get_users(void)
{
    //open file
    FILE *database = fopen(USER_DATABASE, "r");
    if (!database) handle_error("Cannot open database");


    //initialize head of User list
    ListHead *user_head = (ListHead *)malloc(sizeof(ListHead));
    List_init(user_head);

    //read lines and fill the structure's fields
    char buf[BUFFER_LENGTH];
    char username[MAX_LENGTH_USERNAME] = "";
    char password[MAX_LENGTH_PASSWORD] = "";
    size_t buf_len = sizeof(buf);
    while (1)
    {
        //read line
        if (!fgets(buf, buf_len, database))
        {
            if (errno != 0) handle_error("Error while reading from database");
            else break;
        }

        int line_len = strlen(buf);
        if (line_len == 0) break; //stop if reach end
        if (buf[line_len - 1] == '\n') buf[--line_len] = '\0'; //remove escape char for new line

        //get username
        int i;
        for (i = 0; i < line_len; i++)
        {
            if (buf[i] != ',' && buf[i] != '\0') username[i] = buf[i];
            else
            {
                username[i] = '\0';
                ++i;
                break;
            }
        }
        //get password
        int j;
        for (j = 0; i + j < line_len; j++)
        {
            if (buf[i + j] != ',' && buf[i + j] != '\0') password[j] = buf[i + j];
            else
            {
                password[j] = '\0';
                i = i + j + 1;
                break;
            }
        }

        //create new user enter
        UserListItem *userListItem = (UserListItem *)malloc(sizeof(UserListItem));
        if (!userListItem) handle_error("Out of memory!");
        userListItem->listItem.prev = NULL;
        userListItem->listItem.next = NULL;
        strcpy(userListItem->username, username);
        strcpy(userListItem->password, password);
        List_insert(user_head, user_head->last, (ListItem *)userListItem);
    }
    //close file
    fclose(database);
    return user_head;
}

ListHead *database_get_chats(char *username)
{
    //open file
    FILE *database = fopen(CHAT_DATABASE, "r");
    if (!database) handle_error("Cannot open database");

    //initialize head of Chat List
    ChatListHead *chatListHead = (ChatListHead *)malloc(sizeof(ChatListHead));
    strcpy(chatListHead->username, username);
    List_init((ListHead*)chatListHead);

    char buf[BUFFER_LENGTH];
    char first[MAX_LENGTH_USERNAME];
    char second[MAX_LENGTH_USERNAME];
    int line_len;
    while (1)
    {
        //read line
        if (!fgets(buf, sizeof(buf), database))
        {
            if (errno != 0) handle_error("Error while reading from database");
            else break;
        }
        line_len = strlen(buf);
        buf[--line_len] = '\0'; //cut \n escap character
        //find escape char $ to read chat users
        if (!strcmp(buf, "$")) {
            //read first username
            if (!fgets(buf, sizeof(buf), database))
            {
                if (errno != 0) handle_error("Error while reading from database");
                else break;
            }
            line_len = strlen(buf);
            buf[--line_len] = '\0'; //cut \n escap character
            strcpy(first, buf);
            //read second username
            if (!fgets(buf, sizeof(buf), database))
            {
                if (errno != 0) handle_error("Error while reading from database");
                else break;
            }
            line_len = strlen(buf);
            buf[--line_len] = '\0'; //cut \n escap character
            strcpy(second, buf);
            //check if one of them is the one we are looking for
            if (!strcmp(first, username)) {
                ChatListItem* chatListItem = (ChatListItem*)malloc(sizeof(ChatListItem));
                chatListItem->listItem.prev = NULL;
                chatListItem->listItem.next = NULL;
                strcpy(chatListItem->with, second);
                List_insert((ListHead*)chatListHead, chatListHead->listHead.last, (ListItem*)chatListItem);
            }
            else if (!strcmp(second, username)) {
                ChatListItem* chatListItem = (ChatListItem*)malloc(sizeof(ChatListItem));
                chatListItem->listItem.prev = NULL;
                chatListItem->listItem.next = NULL;
                strcpy(chatListItem->with, first);
                List_insert((ListHead*)chatListHead, chatListHead->listHead.last, (ListItem*)chatListItem);
            }
        }
    }

    //close file
    fclose(database);

    return (ListHead*)chatListHead;
}

void database_add_user(ListItem *new_user)
{
    //tranform to user item
    UserListItem *tmp = (UserListItem *)new_user;

    //open database
    FILE *database = fopen(USER_DATABASE, "a+");
    if (!database) handle_error("Cannot open database and add new user");

    //write into database
    fprintf(database, "%s,%s\n", tmp->username, tmp->password);

    //close database
    fclose(database);

    return;
}