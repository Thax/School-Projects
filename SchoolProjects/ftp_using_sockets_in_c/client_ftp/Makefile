GCC = gcc
AFTER_FLAGS = -lm
OBJECTS = client_ftp.o dropper.o packets.o
CLIENT = kftclient
CLEANERS = *.o kftclient

$(CLIENT) : $(OBJECTS)
	$(GCC) $(OBJECTS) -o $(CLIENT) $(AFTER_FLAGS)
    
client.o : client_ftp.c
	$(GCC) -c client_ftp.c $(AFTER_FLAGS)

dropper.o : dropper.c
	$(GCC) -c dropper.c $(AFTER_FLAGS)

packets.o : packets.c
	$(GCC) -c packets.c $(AFTER_FLAGS)

clean:
	rm -f $(CLEANERS)

test:
	./kftclient -d 127.0.0.1 40000 test.txt test.txt 100 5
