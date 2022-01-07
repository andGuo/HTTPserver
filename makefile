EXE = server client
SOBJ = server.o
COBJ = client.o
CC = gcc -Wall

all:    $(EXE)

server: $(SOBJ)
	$(CC) -o server $(SOBJ)

client:	$(COBJ)
	$(CC) -o client $(COBJ)

server.o: server.c common.h
	$(CC) -c server.c

client.o: client.c common.h
	$(CC) -c client.c

clean:
	rm -f $(EXE) $(SOBJ) $(COBJ)
