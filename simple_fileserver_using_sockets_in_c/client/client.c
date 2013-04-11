#include "client.h"

int main(int argc, char *argv[])
{
    // Test for correct number of args (should be 2, 1 user-supplied)
    if (argc != 3) {
        //printf("Incorrect number of arguments supplied.\n Usage: client <server IP addr> <port #>\n");
        exit(0);
    }

    // The IPv4 address of the server.
    char *server_ip = argv[1];

    // The port of the server.  This is defined via argument in the instantiation of the server.
    char *server_port = argv[2];

    // Documentation string to help the user decide commands to pass in.
    char *help_string = "Usage:\n\taddfile: <string> : Adds string listing to the server\n\taddfile -d <string> : deletes listing on the server containing <string>\n\tfilelist : shows all file listings on the server\n\texit : exits the client program\n\n\tCaution!  There is a 50 character limit on strings to send to the server!  Strings longer than this will not be accepted.\n";

    printf("%s", help_string);
    
    // Gives the user the ability to close the program through commands.  The server will still run.
    int exit_flag = 0;

    // Main loop.  Persists until the client chooses to exit the program.
    while (exit_flag == 0) {
        // Prompt
        printf("Input command $ ");

        // Get user input from the console
        char client_input[100];
        fgets(client_input, sizeof(client_input), stdin);

        // String copy to be used if we have to compare against 'addfile'
        char* client_input_copy;
        client_input_copy = client_input;

        // Remove the \n from the string
        client_input[strlen(client_input) - 1] = '\0';

        // Check input command
        if (strcmp(client_input, "filelist") == 0) {
            // Ask the server for a filelist
            char* file_list = askForList(server_ip, server_port);
        } else if (strcmp(client_input, "help") == 0) {
            printf("%s", help_string);
        } else if (strcmp(client_input, "exit") == 0) {
            exit_flag = -1;
        } else {
            // Process the input to extract the command and options
            char* command = strtok(client_input_copy, " ");
            if (strcmp(command, "addfile") == 0) {
                // Copying input to presever memory rules.
                client_input_copy = client_input_copy + 8;
                char parameter_two[strlen(client_input_copy)];
                strcpy(parameter_two, client_input_copy);
                strtok(parameter_two, " ");

                // If the deletion flag is enabled...
                if (strcmp(parameter_two, "-d") == 0) {
                    char* final_parameter = client_input_copy + 3;
                    // Deletes a record from the server matching the string
                    int response = deleteRecord(final_parameter, server_ip, server_port);
                    if (response < 0) {
                        printf("record not deleted.  try again\n");
                        continue;
                    }
                // If the deletion flag is not enabled
                } else {
                    char* final_parameter = client_input_copy;
                    // Send the parameter to the server (adds a record matching the string)
                    printf("adding record: %s\n", final_parameter);
                    int response = addRecord(final_parameter, server_ip, server_port);
                    if (response < 0) {
                        printf("record not added. try again\n");
                    }
                }
            // If the command was none of the above
            } else {
                printf("Invalid command specified.  Type 'help' for valid commands\n");
            }
        }
    }
}

// Function to request a list of all added records.  Prints the list to the console
// Params:  server_ip : The string representation of the ip address of the server we want to connect to
//          server_port : The port on the server that we are connecting to.
char* askForList(char* server_ip, char* server_port) {
    // Instantiate the socket.
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printf("Socket connection failed\n");
        exit(0);
    }

    // Construct the server_address structure
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(server_port));

    // Converting the server ip address to an int
    int converted_ip = inet_pton(AF_INET, server_ip, &server_address.sin_addr.s_addr);
    if (converted_ip == 0) {
        printf("Invalid IP address specified\n");
        exit(0);
    } else if (converted_ip < 0) {
        printf("inet_pton() failed\n");
        exit(0);
    }

    // connect to server
    if (connect(sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        printf("connect() failed.  Try again\n");
        return "1";
    }

    // send request
    ssize_t number_bytes = send(sock, "ls", strlen("ls"), 0);
    if (number_bytes < 0) {
        printf("filelist failed.  Try again.\n");
    } else if (number_bytes != strlen("ls")) {
        printf("Sent unexpected number of bytes.  Try again.\n");
    }

    // recieve response
    char buffer[SERVER_MAX_STRINGS * SERVER_MAX_STRING_SIZE] = "";
    ssize_t recieved_bytes = recv(sock, buffer, (SERVER_MAX_STRINGS * SERVER_MAX_STRING_SIZE) - 1, 0);
    puts(buffer);

    // close socket
    close(sock);
}

// Adds a record to the server
// Params:  record_string : The string we want to add to the server
//          server_ip : The ip of the server we want to connect to
//          server_port : The port on the server we are connecting to
int addRecord(char* record_string, char* server_ip, char* server_port) {
    // Check if the string is too long to add.
    if (strlen(record_string) > 50) {
        printf("String too long.  The server has a limit of 50 characters\n");
        return -1;
    }
    // Instantiate the socket.
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printf("Socket connection failed\n");
        exit(0);
    }

    // Construct the server_address struct.
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(server_port));

    // Converting the server ip address to an int
    int converted_ip = inet_pton(AF_INET, server_ip, &server_address.sin_addr.s_addr);
    if (converted_ip == 0) {
        printf("Invalid IP address specified\n");
        exit(0);
    } else if (converted_ip < 0) {
        printf("inet_pton() failed\n");
        exit(0);
    }

    // connect to server
    if (connect(sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        printf("connect() failed.  Try again\n");
        return 1;
    }

    // Build the actual string and command being sent to the server
    char* full_string = malloc(strlen("add ") + strlen(record_string));
    strcpy(full_string, "add ");
    strcat(full_string, record_string);

    // send request
    ssize_t number_bytes = send(sock, full_string, strlen(full_string), 0);
    if (number_bytes < 0) {
        printf("filelist failed.  Try again.\n");
    } else if (number_bytes != strlen(full_string)) {
        printf("Sent unexpected number of bytes.  Try again.\n");
    }
    //printf("sent\n");

    // recieve response
    char buffer[999] = "";
    ssize_t recieved_bytes = recv(sock, buffer, 998, 0);

    // close socket
    close(sock);
}

// Deletes a record from the server
// Params:  string_to_delete : the string we want to remove from the server.  Removes ALL instances of the string from the server
//          server_ip : the ip address of the server we want to connect to
//          server_port : the port on the server we are connecting to
int deleteRecord(char* string_to_delete, char* server_ip, char* server_port) {
    // Fail if the string is too long to add
    if (strlen(string_to_delete) > 50) {
        printf("String too long.  The server has a limit of 50 characters\n");
        return -1;
    }
    // Instantiate the socket.
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        printf("Socket connection failed\n");
        exit(0);
    }

    // Building the struct for the server_address
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(server_port));

    // Converting the server ip address to an int
    int converted_ip = inet_pton(AF_INET, server_ip, &server_address.sin_addr.s_addr);
    if (converted_ip == 0) {
        printf("Invalid IP address specified\n");
        exit(0);
    } else if (converted_ip < 0) {
        printf("inet_pton() failed\n");
        exit(0);
    }

    // connect to server
    if (connect(sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        printf("connect() failed.  Try again\n");
        return 1;
    }

    // Building the actual string and command being sent to the server
    char* full_string = malloc(strlen("delete ") + strlen(string_to_delete));
    strcpy(full_string, "delete ");
    strcat(full_string, string_to_delete);

    // send request
    ssize_t number_bytes = send(sock, full_string, strlen(full_string), 0);
    if (number_bytes < 0) {
        printf("filelist failed.  Try again.\n");
    } else if (number_bytes != strlen(full_string)) {
        printf("Sent unexpected number of bytes.  Try again.\n");
    }

    // recieve response
    char buffer[999] = "";
    ssize_t recieved_bytes = recv(sock, buffer, 999, 0);
    if (strcmp(buffer, "0") == 0) {
        printf("No such record\n");
    } else {
        printf("%s records deleted.\n", buffer);
    }
    // close socket
    close(sock);
}
