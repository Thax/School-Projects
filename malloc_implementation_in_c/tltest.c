#include "tllist.h"
#include <stdlib.h>
#include <stdio.h>
// Robert Thackston
int main(void)
{
	// Note: Kiddies passing all of these *MAY NOT* guarantee a perfect score.

	// myList sits on the stack.
	List myList = {NULL, 0};

	// Lets add 0
	fprintf(stderr, "Add: %d\n", 0);
	addInOrder(&myList, 0);
	traverse_and_print(&myList);

	// Lets add 9001
	fprintf(stderr, "Add: %d\n", 9001);
	addInOrder(&myList, 9001);
	traverse_and_print(&myList);

	// Lets add 42
	fprintf(stderr, "Add: %d\n", 42);
	addInOrder(&myList, 42);
	traverse_and_print(&myList);

	// Lets add 56
	fprintf(stderr, "Add: %d\n", 56);
	addInOrder(&myList, 56);
	traverse_and_print(&myList);

	// Lets add 10
	fprintf(stderr, "Add: %d\n", 10);
	addInOrder(&myList, 10);
	traverse_and_print(&myList);

	// Kill the list
	emptyList(&myList);
	fprintf(stderr, "Killed the list!\n");
	traverse_and_print(&myList);

	// Lets add 0
	fprintf(stderr, "Add: %d\n", 0);
	addInOrder(&myList, 0);
	traverse_and_print(&myList);

	fprintf(stderr, "Add: %d\n", 1);
	addInOrder(&myList, 1);
	traverse_and_print(&myList);

	fprintf(stderr, "Add: %d\n", 1);
	addInOrder(&myList, 1);
	traverse_and_print(&myList);

	fprintf(stderr, "Add: %d\n", 1);
	addInOrder(&myList, 1);
	traverse_and_print(&myList);

	// Lets add 9001
	fprintf(stderr, "Add: %d\n", 9001);
	addInOrder(&myList, 9001);
	traverse_and_print(&myList);

	emptyList(&myList);

    // Test that adding to the 'front' of the list works.
    fprintf(stderr, "Add: %d\n", 5);
    addInOrder(&myList, 5);
    traverse_and_print(&myList);

    // Add something less than 5.
    fprintf(stderr, "Add: %d\n", 3);
    addInOrder(&myList, 3);
    traverse_and_print(&myList);

    // Add something less than 3.
    fprintf(stderr, "Add: %d\n", 2);
    addInOrder(&myList, 2);
    traverse_and_print(&myList);

    // End tests
    emptyList(&myList);
	return 0;	
}
