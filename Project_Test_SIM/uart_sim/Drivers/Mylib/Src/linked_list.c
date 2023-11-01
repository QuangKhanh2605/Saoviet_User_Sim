#include "linked_list.h"

void InitializeList(LinkedList *list) 
{
	list->head = NULL;
	list->tail = NULL;
	list->current = NULL;
}

void Add_Tail(LinkedList *list, char news[]) 
{
	Node *node = (Node*) malloc(sizeof(Node));
	uint16_t length=strlen(news);
	for(uint16_t i=0;i<length;i++)
	{
		node->Data[i] = news[i];
	}
	node->next = NULL;
	if(list->head == NULL) 
	{
		list->head = node;
	} 
	else 
	{
		list->tail->next = node;
	}
	list->tail = node;
}

void Add_Head(LinkedList *list, char news[]) 
{
	Node *node = (Node*) malloc(sizeof(Node));
	uint16_t length=strlen(news);
	for(uint16_t i=0;i<length;i++)
	{
		node->Data[i] = news[i];
	}
	if (list->head == NULL) 
	{
		list->tail = node;
		node->next = NULL;
	} 
	else 
	{
		node->next = list->head;
	}
	list->head = node;
}

void Delete_Node(LinkedList *list, Node *node) 
{
	if(node == list->head)
	{
		if(list->head->next == NULL) 
		{
			list->head = list->tail = NULL;
		} 
		else 
		{
			list->head = list->head->next;
		}
	} 
	else 
	{
		Node *tmp = list->head;
		while(tmp != NULL && tmp->next != node) 
		{
			tmp = tmp->next;
		}
		if(tmp != NULL)
		{
			tmp->next = node->next;
		}
	}
	free(node);
}

uint32_t Length_LinkedList(LinkedList *list)
{
	int length = 0;
	LinkedList p;
	p=*list;
	while(p.head !=NULL)
	{
		length++;
		p.head = p.head->next;
	}
	return length;
}


