Instructions:

To compile and run the server, enter the directory 'server' and type 'make'

This will run the local makefile and compile the server sourcefiles.  This will create a 'server.exe' file.  (The exe is only there for ease of writing the 'clean' makefile target)

Then, type ./server.exe <port#>.  For example, ./server.exe 40000

This will start the server and it will be waiting for a connection.


To compile and run the client, enter the directory 'client' and type 'make'

This will do exactly what the server makefile does, creating a 'client.exe' file.

Then, type ./client <server_ip> <port#>.  For example, ./client.exe 127.0.0.1 40000

This will start the client and wait for user commands.


Commands:

addfile <text>

	This adds an entry to the server containing the text as the entry.  Example: 'addfile hello' creates entry 'hello'

addfile -d <text>

	This removes any entries to the server containing the text as the entry and corresponding to the client ip.  Example: 'addfile -d hello' removes entry 'hello' if the ip of the entry is the same as the user's ip.

filelist

	This lists all files currently stored on the server. If there are no files on the server, the server will tell you as much.

help

	This reprints the commands, in case the user forgot.

exit

	This exits the client session, and closes the program.  The server will still run.




Description:

The client program translates the user commands into smaller, more specific commands to the server.  'addfile <text>' will send 'add <text>' to the server.  'addfile -d <text> will send 'delete <text>' to the server.  'filelist' will send 'ls' to the server.  In addition, if there are spaces in any of the strings passed by the client, these are maintained, and passed into the server.  The server will then add these strings with spaces, and they can be removed with the same strings.  The server has a limit of 50 characters per string, and although there is no server-side enforcement of this, there is a client side filter that will not send string longer than 50 characters.

The client is non-persistent, in that each time a command is supplied that requires a connection to the server, a new socket and connection is created and closed at the end of the process.  The inherent blocking and multiple-user-handling of the recv() calls are relied upon to manage multiple users.  There is no inherent limit on the number of users that can connect at once.
