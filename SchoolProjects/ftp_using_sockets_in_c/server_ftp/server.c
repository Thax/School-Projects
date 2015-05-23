#include "server.h"

int debug = 0;
int timeout = 0;

int main(int argc, char *argv[])
{
    int arg_offset = 0;
    // Check that the program is being used properly
    if (!(argc > 2 && argc < 5)) {
        printf("Usage: ./kftserver <port #> <loss percentage>\n");
        exit(0);
    } else {
        if (strcmp("-d", argv[1]) == 0) {
            debug = 1;
            arg_offset = 1;
        }
    }

    // The local server port used.
    char *server_port = argv[arg_offset + 1];

    // The percentage of sent packets dropped
    int loss_percent = atoi(argv[arg_offset + 2]);

    // Building the server address structure
    struct addrinfo criteria;
    memset(&criteria, 0, sizeof(criteria));
    criteria.ai_family = AF_INET;
    criteria.ai_flags = AI_PASSIVE;
    criteria.ai_socktype = SOCK_DGRAM;
    criteria.ai_protocol = IPPROTO_UDP;

    struct addrinfo *server_address;
    int ret_val = getaddrinfo(NULL, server_port, &criteria, &server_address);

    // Create socket to listen for incoming connections
    int server_socket = socket(server_address->ai_family, server_address->ai_socktype, server_address->ai_protocol);

    // Bind the socket to the local server address
    if (bind(server_socket, server_address->ai_addr, server_address->ai_addrlen) < 0) {
        printf("bind() failed\n");
        exit(0);
    }

    // Set the drop percentage for recvfrom_dropper
    set_dropper(loss_percent);

    // Serialization Buffer
    unsigned char *serialization_buffer = malloc(sizeof(packet));
    packet recieved_packets;
    packet *packet_buffer = malloc(sizeof(packet));
    int last_packet_num = 0;
    int max_payload_size = 0;
    int old_buffer_length = 0;

    // Setup the handling for timeouts.
    struct sigaction handler;
    handler.sa_handler = handle_alarm;
    if (sigfillset(&handler.sa_mask) < 0) {
        if (debug)
            printf("sigfillset() failed\n");
        exit(0);
    }
    handler.sa_flags = 0;
    if (sigaction(SIGALRM, &handler, 0) < 0) {
        if (debug)
            printf("sigaction() failed\n");
        exit(0);
    }
    // -------------------------------

    char *input_buffer = malloc(65500);
    for (;;) { // We want this to run forever (or until someone kills it)

        struct sockaddr client_address; // Client address

        // Set length of client address struct
        socklen_t client_address_length = sizeof(client_address);

        // Wait for a client to connect
        if (debug)
            printf("Waiting for client on port %d\n", atoi(server_port));

        int num_bytes_recvd = recieve_packet(packet_buffer, server_socket, &client_address, &client_address_length, 65494);

        input_buffer = realloc(input_buffer, 65500);
        memset(input_buffer, 0, 65500);

        max_payload_size = packet_buffer->max_packet_size;

        while (!(packet_buffer->final_packet) && num_bytes_recvd >= 0) {
            // Add the new payload to the end of the buffer.
            old_buffer_length = strlen(input_buffer);
            memcpy(input_buffer + old_buffer_length, packet_buffer->payload, packet_buffer->packet_size);
            last_packet_num = packet_buffer->sequence_number;
            num_bytes_recvd = recieve_packet(packet_buffer, server_socket, &client_address, &client_address_length, max_payload_size);
            if (realloc(input_buffer, old_buffer_length + packet_buffer->packet_size) == NULL) {
                printf("realloc() failed\n");
                exit(0);
            }
        }
        old_buffer_length = strlen(input_buffer);
        memcpy(input_buffer + old_buffer_length, packet_buffer->payload, packet_buffer->packet_size);

        // Read the file from disk if it exists.
        FILE *file_pointer;
        char *output_buffer;
        int file_size = 0;
        if ((file_pointer = fopen(input_buffer, "r")) != NULL) {

            // Get the filesize so we can allocate memory for it.
            fseek(file_pointer, 0, SEEK_END);
            file_size = ftell(file_pointer);
            fseek(file_pointer, 0, SEEK_SET);
            // --------------------------------

            output_buffer = malloc(file_size);
            fread(output_buffer, 1, file_size, file_pointer);
        } else {
            output_buffer = "";
            file_size = 0;
        }
        int data_sent = 0;
        // Send the file contents to the client.
        if (((int)strlen(output_buffer)) > max_payload_size) { // If the size of the string to send is larger than the maximum payload size
            int current_packet_number = 0;
            int current_sequence_number = 1;
            int number_of_packets_to_send = (int)ceil((double)strlen(output_buffer)/(double)max_payload_size);
            int output_buffer_offset = 0;

            struct packet *current_packet = malloc(sizeof(packet));
            current_packet->payload = malloc(max_payload_size);
            while(current_packet_number < number_of_packets_to_send) {
                current_packet = realloc(current_packet, sizeof(packet));
                memset(current_packet, 0, sizeof(packet));
                current_packet->ack = 0;
                current_packet->sequence_number = current_sequence_number;

                output_buffer_offset = max_payload_size * current_packet_number;
                if (current_packet_number == number_of_packets_to_send - 1) {
                    current_packet->packet_size = strlen(output_buffer + output_buffer_offset) + 1; 
                    current_packet->payload = realloc(current_packet->payload, current_packet->packet_size);
                    memset(current_packet->payload, 0, current_packet->packet_size);
                    memcpy(current_packet->payload, output_buffer + output_buffer_offset, strlen(output_buffer + output_buffer_offset) + 1);

                    current_packet->max_packet_size = max_payload_size;
                    current_packet->final_packet = 1;
                } else {
                    current_packet->packet_size = max_payload_size;
                    current_packet->payload = realloc(current_packet->payload, current_packet->packet_size + 1);
                    memset(current_packet->payload, 0, current_packet->packet_size);
                    memcpy(current_packet->payload, output_buffer + output_buffer_offset, max_payload_size);

                    current_packet->max_packet_size = max_payload_size;
                    current_packet->final_packet = 0;
                }

                int bytes_sent = send_packet(*current_packet, server_socket, &client_address, client_address_length);
                data_sent += (int)(current_packet->packet_size);
                printf("Sent %d bytes of %d\n", data_sent, file_size);
                current_packet_number += 1;
                current_sequence_number = !current_sequence_number;
            }
        } else {
            // Send one packet
            struct packet single_packet;
            single_packet.packet_size = strlen(output_buffer);
            single_packet.ack = 0;
            single_packet.sequence_number = 1;
            single_packet.max_packet_size = max_payload_size;
            single_packet.final_packet = 1;
            single_packet.payload = output_buffer;

            int bytes_sent = send_packet(single_packet, server_socket, &client_address, client_address_length);
            if (bytes_sent < 0) {
                printf("bytes_sent() failed.\n");
                exit(0);
            }
        }

        // Clear the buffer for the next request.
        packet_buffer = malloc(sizeof(packet));
    }
}

void handle_alarm() {
    alarm(2);
    timeout += 1;
    if (debug)
        printf("Timed out waiting for ack!\n");
}
