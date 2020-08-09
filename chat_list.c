#include <stdio.h>
#include "chat_list.h"

void ChatList_print(ListHead *head)
{
    ChatListHead* chatListHead = (ChatListHead*)head;
    ListItem *aux = chatListHead->listHead.first;
    printf("[\n%s chat with these awesome people:\n", chatListHead->username);
    while (aux)
    {
        ChatListItem *chatListItem = (ChatListItem *)aux;
        printf("- %s\n", chatListItem->with);
        aux = aux->next;
    }
    printf("]\n");
}