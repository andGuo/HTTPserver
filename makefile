EXE = server client
SOBJ = server.o connect.o
COBJ = client.o connect.o
CC = gcc -Wall

all:    $(EXE)

server: $(SOBJ)
	$(CC) -o server $(SOBJ)

client:	$(COBJ)
	$(CC) -o client $(COBJ)

connect.o: connect.c common.h
	$(CC) -c connect.c

server.o: server.c common.h
	$(CC) -c server.c

client.o: client.c common.h
	$(CC) -c client.c

clean:
	rm -f $(EXE) $(SOBJ) $(COBJ)
