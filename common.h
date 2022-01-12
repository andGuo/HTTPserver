#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <regex.h>

#define PORT 8080
#define STR_PORT "8080"
#define MAX_BACKLOG 5
#define MAX_BUFFER_SIZE 2048

/* Function forward references */

//server.c
void serveOneClient(int clientFd);

//client.c
void doClientRequest(int serverFd);

//connect.c
int errorCheck(int rtnVal, const char *message);
void setUpServer(int *serverSok);
void acceptConnect(int serverSok, int *clientSok);
void connectClient(int *serverSok);

//clientHttp.c
void sendSimpleRequest(const char* document, int serverFd);
void sendFullRequest();
void handleResponse(int serverFd);