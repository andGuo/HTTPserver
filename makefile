EXE = server client
SOBJ = server.o connect.o serverHttp.o util.o queue.o
COBJ = client.o connect.o clientHttp.o util.o
CC = gcc
CFLAG = -Wall
LIB = -pthread

all:    $(EXE)

server: $(SOBJ)
	$(CC) $(CFLAG) -o server $(SOBJ) $(LIB)

client:	$(COBJ)
	$(CC) $(CFLAG) -o client $(COBJ)

connect.o: connect.c common.h
	$(CC) $(CFLAG) -c connect.c

server.o: server.c common.h
	$(CC) $(CFLAG) -c server.c

serverHttp.o: serverHttp.c common.h
	$(CC) $(CFLAG) -c serverHttp.c

client.o: client.c common.h
	$(CC) $(CFLAG) -c client.c

clientHttp.o: clientHttp.c common.h
	$(CC) $(CFLAG) -c clientHttp.c

util.o: util.c common.h
	$(CC) $(CFLAG) -c util.c

queue.o: queue.c common.h
	$(CC) $(CFLAG) -c queue.c

clean:
	rm -f $(EXE) $(SOBJ) $(COBJ)
