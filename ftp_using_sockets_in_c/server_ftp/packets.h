#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <netdb.h> 
#include <math.h> 
#include <errno.h> 
#include <signal.h> 
#include "dropper.h"

typedef struct packet
{
    size_t packet_size;
    char ack;
    char sequence_number;
    char final_packet;
    size_t max_packet_size;
    char* payload;
} packet;

#define TIMEOUT_TIME (1)
#define MAX_TIMEOUT (5)

void serialize_packet(unsigned char *buffer, packet server_packet);
void unserialize_packet(unsigned char *buffer, packet *server_packet);
int send_ack(int sock, struct sockaddr *dest_addr, socklen_t dest_addr_len);
int recieve_ack(int sock, struct sockaddr *from_addr, socklen_t *from_addr_len);
int send_packet(packet packet_to_send, int sock, const struct sockaddr *dest_addr, socklen_t dest_addr_len);
int recieve_packet(packet *packet_to_recieve, int sock, struct sockaddr *from_addr, socklen_t *from_addr_len, int max_packet_length);

