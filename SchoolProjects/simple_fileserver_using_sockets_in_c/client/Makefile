GCC = gcc
OBJECTS = client.o
CLIENT = client.exe
CLEANERS = *.o *.exe

$(CLIENT) : $(OBJECTS)
	$(GCC) $(OBJECTS) -o $(CLIENT)
    
client.o : client.c
	$(GCC) -c client.c

clean:
	rm -f $(CLEANERS)
