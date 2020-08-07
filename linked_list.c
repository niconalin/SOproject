#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

void List_init(ListHead *head)
{
    head->first = 0;
    head->last = 0;
    head->size = 0;
}

ListItem *List_find(ListHead *head, ListItem *item)
{
    ListItem *aux = head->first;
    while (aux)
    {
        if (aux == item)
            return item;
        aux = aux->next;
    }
    return 0;
}

ListItem *List_insert(ListHead *head, ListItem *prev, ListItem *item)
{
    if (item->next || item->prev)
        return 0;
    ListItem *next = prev ? prev->next : head->first;
    if (prev)
    {
        item->prev = prev;
        prev->next = item;
    }
    if (next)
    {
        item->next = next;
        next->prev = item;
    }
    if (!prev)
        head->first = item;
    if (!next)
        head->last = item;
    ++head->size;
    return item;
}

ListItem *List_detach(ListHead *head, ListItem *item)
{
    ListItem *prev = item->prev;
    ListItem *next = item->next;
    if (prev)
    {
        prev->next = next;
    }
    if (next)
    {
        next->prev = prev;
    }
    if (item == head->first)
        head->first = next;
    if (item == head->last)
        head->last = prev;
    head->size--;
    item->next = item->prev = 0;
    return item;
}

void List_free(ListHead *head)
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