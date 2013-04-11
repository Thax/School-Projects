#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SERVER_MAX_STRINGS (50)
#define SERVER_MAX_STRING_SIZE (100)

int addRecord(char*, char*, char*);
int deleteRecord(char*, char*, char*);
char* askForList(char*, char*);
