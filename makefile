EXE = server client
SOBJ = server.o connect.o serverHttp.o util.o queue.o
COBJ = client.o connect.o clientHttp.o util.o
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

serverHttp.o: serverHttp.c common.h
	$(CC) -c serverHttp.c

client.o: client.c common.h
	$(CC) -c client.c

clientHttp.o: clientHttp.c common.h
	$(CC) -c clientHttp.c

util.o: util.c common.h
	$(CC) -c util.c

queue.o: queue.c common.h
	$(CC) -c queue.c

clean:
	rm -f $(EXE) $(SOBJ) $(COBJ)
