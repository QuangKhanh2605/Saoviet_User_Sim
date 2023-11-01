#ifndef __LINKED_LIST_
#define __LINKED_LIST_

#include "stdlib.h"
#include "stm32l1xx_hal.h"
#include "string.h"

typedef struct _node {
    char Data[30];
    struct _node *next;
} Node;

typedef struct _linkedLidt {
    Node *head;
    Node *tail;
    Node *current;
} LinkedList;

void InitializeList(LinkedList *list);

void Add_Head(LinkedList *list, char news[]);
void Add_Tail(LinkedList *list, char news[]);

uint32_t Length_LinkedList(LinkedList *list);
void Delete_Node(LinkedList *list, Node *node);

#endif

