GCC = gcc
AFTER_FLAGS = -lm
OBJECTS = server.o dropper.o packets.o
SERVER = kftserver
CLEANERS = *.o kftserver

$(SERVER) : $(OBJECTS)
	$(GCC) $(OBJECTS) -o $(SERVER) $(AFTER_FLAGS)
    
server.o : server.c
	$(GCC) -c server.c $(AFTER_FLAGS)

dropper.o : dropper.c
	$(GCC) -c dropper.c $(AFTER_FLAGS)

packets.o : packets.c
	$(GCC) -c packets.c $(AFTER_FLAGS)

clean:
	rm -f $(CLEANERS)

test-mem:
	valgrind -v --tool=memcheck --leak-check=yes ./kftserver 40000 0
