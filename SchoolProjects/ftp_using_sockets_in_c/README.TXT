To Run the Client:

cd to the client_ftp directory

run 'make' to compile the code

To run the executable, enter a command in the following format:

./client_ftp [-d] <server IP addr> <port #> <remote_filename> <local_filename> <max_packet_size> <loss_percent>

Alternatively, to simply download the small test.txt file, run make test.

NOTE: The server must be running before running the client. Ensure that the server is running on port 40000 when running make test.


To Run the Server:

cd to the server_ftp directory

run make to compile the code

To run the executable, enter a command in the following format:

./server_ftp [-d] <port #> <loss_percent>




ARQ Protocol:

The client program streams packets to the server in the following format:

Bytes (1, 4): Payload size
Byte 5: ACK flag
Byte 6: Sequence number
Byte 7: Final packet flag
Bytes (8, 11): Maximum payload size
Bytes (12, Payload size - 11): Payload

This is created as a struct, then serialized and sent to the server.

The client then begins to listen for an ack (packet with the ack flag toggled), with a 1 second window.

If the server does not respond within this window, the client resends the packet.

If the client recieves an ack, it sends the next packet, with a flipped sequence number bit.

It does this until it sends the final packet (with a toggled finap packet flag) and recieves the ack for it.

Then, the client will begin to recieve the data packets from the server.

When it recieves a packet, it responds with an ack, adds the payload to a buffer, and begins to listen for the next packet.

If the next packet has the same sequence number as the preceeding one, the client adds nothing, but sends an ack anyway.

It does this until it recieves a packet with a final packet flag toggled, then saves the entire buffer to a file and exits.



When the server begins to recieve packets, it does much the same as the client.

When it recieves a packet that it didn't just recieve, it saves it to a buffer, sends an ack and continues to listen.

When it recieves a 'final packet' packet, it sends its final ack and moves on.

At this point, it reads the information from the file the client wanted, and begins to break it up into packets.

It sends these in the exact same process the client used, and does so until it sends the final packet.

If it does not recieve the final ack after 5 retries, it times out and resets to listen for a new connection.




Limitations:

The biggest limitation of this implementation is the memory footprint. Many 'mallocs' are called with large values, for the sake of reliability.

