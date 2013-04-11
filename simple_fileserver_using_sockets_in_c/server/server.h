#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>

static int MAXPENDING = 10; // Maximum outstanding connection requests TODO: Find out if this applies
#define MAXSTRINGS 100 // Maximum number of strings stored at any given time.
char *stored_strings[MAXSTRINGS]; // Array storing all of the strings from clients.
char *stored_ip_addresses[MAXSTRINGS]; // Array storing all of the ip_addresses from clients.

char* process_response(char*);
int find_index_containing_string(char*, char*);
void shrink_arrays(int);
char* get_list();
void add_record_to_array(char*, char*);
int remove_record_from_array(char*, char*);
