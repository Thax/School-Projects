GCC = gcc
OBJECTS = server.o
SERVER = server.exe
CLEANERS = *.o *.exe

$(SERVER) : $(OBJECTS)
	$(GCC) $(OBJECTS) -o $(SERVER)
    
server.o : server.c
	$(GCC) -c server.c

clean:
	rm -f $(CLEANERS)
