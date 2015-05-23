#include "server.h"

// Array for storing the strings
char string_array[100][50];
// Array for storing the ip_addresses
char ip_array[100][50];
// The current count of records in the array
int array_count = 0;

int main(int argc, char *argv[])
{
    // Check that the program is being used properly
    if (argc != 2) {
        printf("Usage: ./server <port #>\n");
        exit(0);
    }

    in_port_t server_port = atoi(argv[1]); // The local port the server communicates through

    // Create socket to listen for incoming connections
    int server_socket; // Socket descriptor
    if ((server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        printf("socket() failed\n");
        exit(0);
    }

    // Create local address struct
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(server_port);

    // Bind the socket to the local server address
    if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        printf("bind() failed\n");
        exit(0);
    }

    // Mark the socket to listen for incoming connections
    if (listen(server_socket, MAXPENDING) < 0) {
        printf("listen() failed\n");
        exit(0);
    }

    for (;;) { // We want this to run forever (or until someone kills it)
        struct sockaddr_in client_address; // Client address

        // Set length of client address struct
        socklen_t client_address_length = sizeof(client_address);

        // Wait for a client to connect
        printf("Waiting for client on port %d\n", server_port);
        int client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_address_length);
        if (client_socket < 0) {
            printf("accept() failed\n");
            exit(0);
        }

        char client_name[INET_ADDRSTRLEN]; // Storing the address of the client
        if (inet_ntop(AF_INET, &client_address.sin_addr.s_addr, client_name, sizeof(client_name))) {
            printf("Handling client with IP address: %s\n", client_name);
        } else {
            printf("Unable to get client address\n");
        }

        // Recieve the command from the client
        char buffer[100] = "";
        ssize_t bytes_recieved = recv(client_socket, buffer, 100, 0);
        if (bytes_recieved < 0) {
            ssize_t bytes_sent = send(client_socket, "recv() failed. Try your command again.\n", strlen("recv() failed. Try your command again\n"), 0);
            continue;
        }

        // Copy the buffer to follow memory rules
        char* buffer_copy;
        buffer_copy = buffer;

        // If command recieved is to list files
        if (strcmp(buffer_copy, "ls") == 0) {
            // If there are no files
            if (array_count == 0) {
                ssize_t bytes_sent = send(client_socket, "Server empty\n", strlen("Server empty\n"), 0);
                if (bytes_sent < 0) {
                    printf("Sending list of strings failed\n");
                }
            // If there are files
            } else {
                char* string_to_send = get_list(string_array, ip_array, array_count);
                ssize_t bytes_sent = send(client_socket, string_to_send, strlen(string_to_send), 0);
                free(string_to_send);
                if (bytes_sent < 0) {
                    printf("Sending list of strings failed\n");
                }
            }
        // If command is not to list files
        } else {
            char* command = strtok(buffer_copy, " ");
            // If command is to add record
            if (strcmp(command, "add") == 0) {
                buffer_copy = buffer_copy + 4;
                add_record_to_array(buffer_copy, client_name);
                ssize_t bytes_sent = send(client_socket, "Record Added\n", strlen("Record Added\n"), 0);
                if (bytes_sent < 0) {
                    printf("Sending record add confirmation failed\n");
                }
            // If command is to delete record(s)
            } else if (strcmp(command, "delete") == 0) {
                buffer_copy = buffer_copy + 7;
                int num_records_deleted = remove_record_from_array(buffer_copy, client_name);
                char ret_string[100];
                sprintf(ret_string, "%d", num_records_deleted);
                ssize_t bytes_sent = send(client_socket, ret_string, strlen(ret_string), 0);
                if (bytes_sent < 0) {
                    printf("Sending record delete confirmation failed\n");
                }
            // If command is invalid.  THIS SHOULD NEVER HAPPEN
            } else {
                char* super_error_string = "You managed to send a bad string to the server.  Well done\n";
                ssize_t bytes_send = send(client_socket, super_error_string, strlen(super_error_string), 0);
            }
        }
    }
}

// Builds and returns a formatted list of all the records in the server
char* get_list() {
    int count;
    char* ret_string = malloc(sizeof("IP\t\tSTRING\n"));
    memmove(&ret_string[0], "IP\t\tSTRING\n", strlen("IP\t\tSTRING\n"));
    size_t tab_len = strlen("\t");
    size_t newline_len = strlen("\n");
    for (count = 0; count < array_count; count++) {
        size_t ret_string_len = strlen(ret_string);
        size_t ip_array_len = strlen(ip_array[count]);
        size_t string_array_len = strlen(string_array[count]);

        ret_string = realloc(ret_string, ret_string_len + ip_array_len + tab_len + string_array_len + newline_len);
        memmove(&ret_string[0] + ret_string_len, ip_array[count], ip_array_len);
        ret_string_len = ret_string_len + ip_array_len;
        memmove(&ret_string[0] + ret_string_len, "\t", tab_len);
        ret_string_len = ret_string_len + tab_len;
        memmove(&ret_string[0] + ret_string_len, string_array[count], string_array_len + 1);
        ret_string_len = ret_string_len + string_array_len;
        memmove(&ret_string[0] + ret_string_len, "\n", newline_len + 1);
        ret_string_len = ret_string_len + newline_len;
    }
    return ret_string;
}

// Adds a single record to the string_array, and a corresponding ip address to the ip_array
// Params:  string_to_add : the string to add to the array
//          client_ip : the ip address to add to the array
void add_record_to_array(char* string_to_add, char* client_ip) {
    strncpy(string_array[array_count], string_to_add, strlen(string_to_add));
    strcpy(ip_array[array_count], client_ip);
    array_count = array_count + 1;
}

// Searches for and removes records and ip addresses that correspond to the string passed in and the ip of the client asking
// Params:  string_to_remove : The string to remove from the server
//          client_ip : The ip of the client asking
int remove_record_from_array(char* string_to_remove, char* client_ip) {
    int deletion_index = 0;
    int number_of_records_deleted = 0;
    while (deletion_index != -1) {
        deletion_index = find_index_containing_string(string_to_remove, client_ip);
        if (deletion_index != -1) {
            number_of_records_deleted = number_of_records_deleted + 1;
            shrink_arrays(deletion_index);
        }
    }
    return number_of_records_deleted;
}

// Removes entry from the array and shrinks it to compensate.
// Params:  index_to_remove : the index of the entry we want to remove.
void shrink_arrays(int index_to_remove) {
    int count;
    for (count = index_to_remove; count < array_count; count++) {
        strcpy(string_array[count], string_array[count + 1]);
        strcpy(ip_array[count], ip_array[count + 1]);
    }
    array_count = array_count - 1;
}

// Returns the first index that contains the string
// Params:  string : the string we want to find the index for.
//          client_ip : the ip of the client asking.  we only remove records that correspond to this ip.
int find_index_containing_string(char* string, char* client_ip) {
    int count;
    for (count = 0; count < array_count; count++) {
        if (strncmp(string_array[count], string, strlen(string) - 1) == 0) {
            if (strcmp(ip_array[count], client_ip) == 0) {
                return count;
            }
        }
    }
    return -1;
}
