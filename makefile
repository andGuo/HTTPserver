EXE = server client
SOBJ = server.o connect.o serverHttp.o
COBJ = client.o connect.o clientHttp.o
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

serverHttp.0: serverHttp.c common.server
	$(CC) -c serverHttp.c

client.o: client.c common.h
	$(CC) -c client.c

clientHttp: clientHttp.c common.clientHttp
	$(CC) -c clientHttp.c

clean:
	rm -f $(EXE) $(SOBJ) $(COBJ)
