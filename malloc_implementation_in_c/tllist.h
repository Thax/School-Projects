#ifndef TLLIST_H
#define TLLIST_H

typedef struct node
{
	struct node* next;
	int data; /* Notice how this is NOT a pointer */
} Node;

typedef struct list
{
	Node* head;
	unsigned int size;
} List;

void addInOrder(List* head, int value);
void emptyList(List* head);
void traverse_and_print(List* myList);

#endif
