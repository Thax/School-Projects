#include "client_ftp.h"

int debug = 0; // Flag for determining if debug information is printed.
int timeout_tries = 0;

int main(int argc, char *argv[])
{
    int args_offset = 0;
    // Test for correct number of args (should be 7, 6 user-supplied)
    if (argc > 8 || argc < 7) {
        printf("Incorrect number of arguments supplied.\n Usage: client_ftp [-d] <server IP addr> <port #>");
        printf("<remote_filename> <local_filename> <max_packet_size> <loss_percent>\n");
        exit(0);
    } else {
        if (strcmp("-d", argv[1]) == 0) {
            debug = 1;
            args_offset = 1;
        }
    }

    // The IPv4 address of the server.
    char *server_ip = argv[args_offset + 1];

    if (inet_pton(AF_INET, server_ip, malloc(4)) < 0) {
        printf("invlaid IP address\n");
        exit(0);
    }
    if (debug)
        printf("Using server ip %s\n", server_ip);

    // The port of the server.  This is defined via argument in the instantiation of the server.
    char *server_port = argv[args_offset + 2];
    if (debug)
        printf("Using server port %s\n", server_port);

    // The filename on the server.
    char *remote_filename = argv[args_offset + 3];
    if (debug)
        printf("Looking for file name %s\n", remote_filename);

    // The filename that will be used locally
    char *local_filename = argv[args_offset + 4];
    if (debug)
        printf("Saving as file name %s\n", local_filename);

    // The largest payload size used in the packets generated
    int max_packet_size = atoi(argv[args_offset + 5]);
    if (debug)
        printf("Maximum payload size of packet is: %d\n", max_packet_size);

    // The percentage packet loss simulated
    int loss_percent = atoi(argv[args_offset + 6]);
    if (!((loss_percent < 100) && (loss_percent >= 0))) {
        printf("invalid loss percentage designated\n");
        exit(0);
    }

    if (debug)
        printf("Using loss percentage of %d%%\n", loss_percent);

    // Get information about the server we're looking for
    struct addrinfo criteria;
    memset(&criteria, 0, sizeof(criteria));
    criteria.ai_family = AF_INET;
    criteria.ai_socktype = SOCK_DGRAM;
    criteria.ai_protocol = IPPROTO_UDP;
    // --------------------------------------------------

    // Get the address --------
    struct addrinfo *server_address_info;
    int ret_val = getaddrinfo(server_ip, server_port, &criteria, &server_address_info);
    // ------------------------

    // Create the socket ------
    int sock = socket(server_address_info->ai_family,
                      server_address_info->ai_socktype,
                      server_address_info->ai_protocol
                      );
    // -------------------------

    // Set the dropper percentage
    set_dropper(loss_percent);
    if (debug)
        printf("Dropper percentage set to %d\n", loss_percent);
    // --------------------------

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
    struct timeval *tv_start = malloc(sizeof(struct timeval));

    // Create the packets to be sent.
    if (strlen(remote_filename) > max_packet_size) { // If the size of the string to send is larger than the maximum payload size
        // Instantiate utility variables
        int current_packet_number = 0;
        int current_sequence_number = 1;
        int number_of_packets_to_send = (int)ceil((double)strlen(remote_filename)/(double)max_packet_size);
        // -----------------------------

        // Create and send all the packets we need.
        gettimeofday(tv_start, NULL);
        while(current_packet_number < number_of_packets_to_send) {
            struct packet *current_packet = malloc(sizeof(packet));
            current_packet->ack = 0; // None of these are acknowledgement packets
            current_packet->sequence_number = current_sequence_number; // Apply the current sequence number to the packet

            int remote_filename_offset = max_packet_size * current_packet_number;
            if (current_packet_number == number_of_packets_to_send - 1) { // If we are on the last packet to send
                current_packet->packet_size = strlen(remote_filename + remote_filename_offset); 
                current_packet->payload = malloc(current_packet->packet_size);
                memcpy(current_packet->payload, remote_filename + remote_filename_offset, strlen(remote_filename + remote_filename_offset));
                current_packet->final_packet = 1;
            } else { // If we are not on the last packet to send
                current_packet->payload = malloc(max_packet_size);
                memcpy(current_packet->payload, remote_filename + remote_filename_offset, max_packet_size);
                current_packet->final_packet = 0;
                current_packet->packet_size = max_packet_size;
            }

            // Send the packet we just built
            if (debug)
                printf("Sending packet %d of %d\n", current_packet_number + 1, number_of_packets_to_send);
            int bytes_sent = send_packet(*current_packet, sock, server_address_info->ai_addr, server_address_info->ai_addrlen);
            // -----------------------------

            current_packet_number += 1;
            current_sequence_number = !current_sequence_number;
        }
    } else { // If the packet can fit in one packet
        gettimeofday(tv_start, NULL);

        // Create our single packet
        struct packet single_packet;
        single_packet.packet_size = strlen(remote_filename);
        single_packet.ack = 0;
        single_packet.sequence_number = 1;
        single_packet.max_packet_size = max_packet_size;
        single_packet.final_packet = 1;
        single_packet.payload = remote_filename;
        // -----------------------

        // Send the packet we just created
        int bytes_sent = send_packet(single_packet, sock, server_address_info->ai_addr, server_address_info->ai_addrlen);
        if (bytes_sent < 0) {
            if (debug)
                printf("bytes_sent() failed.\n");
            exit(0);
        }
        // -------------------------------
    }
    int last_packet_num = 0;
    packet recieved_packets;
    packet *packet_buffer = malloc(sizeof(packet));

    // Create the file if it doesn't exist, otherwise wipe it clean.
    FILE *out_file;
    out_file = fopen(local_filename, "w");
    fclose(out_file);
    while (1) { // We start recieving the file from the server
        // Wait for the server to start sending the file.
        if (debug)
            printf("Waiting for server on port %d\n", atoi(server_port));
        int num_bytes_recvd = recieve_packet(packet_buffer, sock, server_address_info->ai_addr, &server_address_info->ai_addrlen, max_packet_size);
        // ----------------------------------------------

        // If we are dealing with a fresh packet
        if (last_packet_num != packet_buffer->sequence_number) {

            if (num_bytes_recvd < 0) {
                if (debug)
                    printf("recvfrom() failed\n");
                exit(0);
            }
            // Add the contents to a file
            out_file = fopen(local_filename, "a+");
            fprintf(out_file, "%s", packet_buffer->payload);
            fclose(out_file);
            // --------------------------
        }
        // Update the sequence number, so that we don't add the packet if we get a copy of the last one
        last_packet_num = packet_buffer->sequence_number;

        // If this was the final packet, break out of the loop
        if (packet_buffer->final_packet) {
            break;
        }
        // ----------------------------------------------------
    }
    struct timeval *tv_end = malloc(sizeof(struct timeval));
    gettimeofday(tv_end, NULL);
    int total_seconds = tv_end->tv_sec - tv_start->tv_sec;
    int total_mseconds = tv_end->tv_usec - tv_start->tv_usec;
    printint(total_seconds);
    printint(total_mseconds);
    close(sock);
}

void printint(int int_to_print) {
    printf("%d\n", int_to_print);
}

void handle_alarm() {
    alarm(TIMEOUT_TIME);
    if (debug)
        printf("Timed out waiting for ack!\n");
}
