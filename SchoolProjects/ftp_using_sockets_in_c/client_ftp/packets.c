#include "packets.h"

extern int debug;
extern int timeout;

int send_packet(packet packet_to_send, int sock, const struct sockaddr *dest_addr, socklen_t dest_addr_len) {
    // Utility variables
    int ack_successful = 0;
    int bytes_sent = 0;
    int bytes_recieved = 0;
    // -----------------

    // Create the buffer for storing serialized packets to send.
    unsigned char *serialization_buffer = malloc(65500);

    // Serialize the packet for transmission
    serialize_packet(serialization_buffer, packet_to_send);
    // Send and resend the packet until we get an ack from the peer.
    while (!ack_successful) {
        bytes_sent = sendto_dropper(sock, 
                                    serialization_buffer,
                                    11 + packet_to_send.packet_size,
                                    0,
                                    dest_addr,
                                    dest_addr_len
                                    );
        if (bytes_sent < 0) {
            if (debug)
                printf("sendto_dropper() failed.\n");
            exit(0);
        }
        // Wait for ack from the server.
        struct sockaddr from_addr;
        socklen_t from_addr_len = sizeof(from_addr);
        bytes_recieved = recieve_ack(sock, &from_addr, &from_addr_len);
        if (bytes_recieved > 0) {
            ack_successful = 1;
        }
    }
    return bytes_sent;
}

int send_ack(int sock, struct sockaddr *dest_addr, socklen_t dest_addr_len) {
    // Utility variables
    int bytes_sent = 0;
    // -----------------

    // Build the ack packet
    packet ack_packet;
    ack_packet.packet_size = 0;
    ack_packet.ack = 1;
    ack_packet.max_packet_size = 0;
    ack_packet.sequence_number = 0;
    ack_packet.final_packet = 1;
    ack_packet.payload = "";
    // --------------------

    // Create the buffer for storing serialized packets to send.
    unsigned char *serialization_buffer = malloc(65500);

    // Serialize the packet for transmission
    serialize_packet(serialization_buffer, ack_packet);

    // Send the ack.  We don't need to worry about getting an ack for our ack.
    bytes_sent = sendto_dropper(sock, 
                                serialization_buffer,
                                11,
                                0,
                                dest_addr,
                                dest_addr_len
                                );
    if (bytes_sent < 0) {
        if (debug)
            printf("sendto_dropper() failed.\n");
        exit(0);
    }
    return bytes_sent;
}

int recieve_packet(packet *packet_to_recieve, int sock, struct sockaddr *from_addr, socklen_t *from_addr_len, int max_packet_size) {
    char *packet_buffer = malloc(65500);
    int num_bytes_recvd = 0;
    num_bytes_recvd = recvfrom(sock,
                               packet_buffer,
                               max_packet_size + 11,
                               0,
                               from_addr,
                               from_addr_len
                               );
    unserialize_packet(packet_buffer, packet_to_recieve);
    free(packet_buffer);
    if (packet_to_recieve->ack) {
        return 0;
    }
    int bytes_sent = send_ack(sock, from_addr, *from_addr_len);
    if (bytes_sent < 0) {
        printf("sending ack failed in recieve packet\n");
        exit(0);
    }
    return num_bytes_recvd;
}

int recieve_ack(int sock, struct sockaddr *from_addr, socklen_t *from_addr_len) {
    char *packet_buffer = malloc(sizeof(packet) + 1);
    int num_bytes_recvd = 0;
    alarm(TIMEOUT_TIME);
    while ((num_bytes_recvd = recvfrom(sock,
                                       packet_buffer, 
                                       sizeof(packet) + 1, 
                                       0, 
                                       from_addr, 
                                       from_addr_len
                                       )) < 0) {
        if (errno == EINTR) { // If it was a timeout, return 0
            return 0;
        } else if (num_bytes_recvd < 0) {
            if (debug)
                printf("Recieving ack failed\n");
            exit(0);
        }
    }
    alarm(0);
    packet *packet_to_recieve = malloc(sizeof(packet));
    unserialize_packet(packet_buffer, packet_to_recieve);
    if (packet_to_recieve->ack == 0) {
        return 0;
    }
    free(packet_buffer);
    return num_bytes_recvd;
}

void serialize_packet(unsigned char *buffer, struct packet server_packet) {
    unsigned char *temp_buffer = buffer; // So I can reset the buffer at the end without worrying about garbage collection.

    int serialized_packet_size = htonl(server_packet.packet_size);
    memcpy(temp_buffer, &serialized_packet_size, 4);
    temp_buffer += 4;

    temp_buffer[0] = server_packet.ack;
    temp_buffer += 1;

    temp_buffer[0] = server_packet.sequence_number;
    temp_buffer += 1;

    temp_buffer[0] = server_packet.final_packet;
    temp_buffer += 1;

    int serialized_max_packet_size = htonl(server_packet.max_packet_size);
    memcpy(temp_buffer, &serialized_max_packet_size, 4);
    temp_buffer += 4;

    memcpy(temp_buffer, server_packet.payload, server_packet.packet_size + 1);
}

void unserialize_packet(unsigned char *buffer, struct packet *local_packet) {
    unsigned char *temp_buffer = buffer; // So I can reset the buffer at the end of the fcn without worrying about garbage collection.
    // Unserialize the packet size field
    int packet_size;
    memcpy(&packet_size, temp_buffer, 4);
    local_packet->packet_size = ntohl(packet_size);
    temp_buffer += 4; // Increment the temp_buffer pointer to point to the next field.

    // Unserialize the ack field
    local_packet->ack = *temp_buffer;
    temp_buffer += 1; // Increment the temp_buffer pointer to point to the next field.

    // Unserialize the sequence number field
    local_packet->sequence_number = *temp_buffer;
    temp_buffer += 1; // Increment the temp_buffer pointer to point to the next field.

    // Unserialize the final packet field
    local_packet->final_packet = *temp_buffer;
    temp_buffer += 1; // Increment the temp_buffer pointer to point to the next field.

    // Unserialize the maximum packet size field
    int max_packet_size;
    memcpy(&max_packet_size, temp_buffer, 4);
    local_packet->max_packet_size = ntohl(max_packet_size);
    temp_buffer += 4; // Increment the temp_buffer pointer to point to the next field.

    // Unserialize the payload field
    local_packet->payload = malloc(local_packet->packet_size + 1);
    memcpy(local_packet->payload, temp_buffer, local_packet->packet_size + 1);
}
