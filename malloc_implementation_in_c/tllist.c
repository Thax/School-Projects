#include "tllist.h"
#include <stdio.h>
#include <stdlib.h>
// Robert Thackston
/**  Timed Lab 6 - Linked Lists
  *
  */

/** addInOrder
  *
  * Adds the data in the linked list in the correct position.
  * Example addInOrder(1 -> 2 -> 4, 3) will result in the list 1 -> 2 -> 3 -> 4
  * Example addInOrder(1 -> 2 -> 4, 0) will result in the list 0 -> 1 -> 2 -> 4
  *
  */
void addInOrder(List* mylist, int value)
{
 	if (mylist->size == 0) {
        mylist->head = malloc(sizeof(Node));
        mylist->head->data = value;
        mylist->head->next = NULL;
        mylist->size = 1;
        return;
    }
    if (value <= mylist->head->data) {
        Node* newNode = malloc(sizeof(Node));
        newNode->data = value;
        newNode->next = mylist->head;
        mylist->head = newNode;
        mylist->size++;
        return;
    }
    Node* comparisonCursor = mylist->head;
    while (comparisonCursor->next != NULL) {
        comparisonCursor = comparisonCursor->next;
    }
    if (comparisonCursor->data < value) {
        comparisonCursor->next = malloc(sizeof(Node));
        comparisonCursor->next->data = value;
        comparisonCursor->next->next = NULL;
        mylist->size++;
        return;
    }
    Node* cursor1;
    Node* cursor2;
    cursor2 = mylist->head;
    for (int i = 0; i < mylist->size; i++) {
        if (value > cursor2->data) {
            cursor1 = cursor2;
            cursor2 = cursor2->next;
        } else {
            cursor1->next = malloc(sizeof(Node));
            cursor1->next->data = value;
            cursor1->next->next = cursor2;
            mylist->size++;
            return;
        }
    }
}

/** emptyList
  *
  * Frees the entire linked list and makes it EMPTY (Remember what constitutes an empty list!)
  * Example emptyList(1 -> 2 -> 2 -> 3) will result in an empty list.
  * 
  * Remember to free everything that is necessary.
  */
void emptyList(List* mylist)
{
    if (mylist->size == 0) { // Nothing needs to be done
        return;
    }
    if (mylist->size == 1) {
        free(mylist->head);
        mylist->head = NULL;
        mylist->size = 0;
        return;
    }
    Node* cursor1;
    Node* cursor2;
    cursor1 = mylist->head;
    cursor2 = mylist->head->next;
    while (cursor2 != NULL) {
        free(cursor1);
        cursor1 = cursor2;
        cursor2 = cursor2->next;
    }
    free(cursor1);
    mylist->size = 0;
    mylist->head = NULL;
}

/** traverse_and_print
  * Traverses the list and prints out each int stored in the linked list.
  * Example traverse_and_print(1 -> 2 -> 2 -> 3) will result in the following output: "1 2 2 3\n" (minus the quotes)
  *         traverse_and_print(Empty List) will result in the following output: "\n" (minus the quotes)
  */
void traverse_and_print(List* mylist)
{
    if (mylist->size == 0) {
        printf("\n");
        return;
    }
    Node* cursor = mylist->head;
    printf("%d", mylist->head->data);
    while (cursor->next != NULL) {
        cursor = cursor->next;
        printf(" %d", cursor->data);
    }
    printf("\n");
}
