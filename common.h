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
#define MAX_STRING 256
#define MAX_REQUEST 16

typedef enum {get, head, post} RequestEnum;
static const char *REQUEST[] = {"GET", "HEAD", "POST"};

typedef struct {
    char method[MAX_REQUEST];
    char uri[MAX_BUFFER_SIZE];
    char httpVer[MAX_REQUEST];
    int socket;
} requestType;

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
void *getInAddr(struct sockaddr *sa);

//clientHttp.c
void sendSimpleRequest(const char* document, int serverFd);
void sendFullRequest();
void handleResponse(int serverFd);

//serverHttp.c
int handleRequest(char *buffer, int clientFd);
void respond(requestType *r);
void createHeader(RequestEnum rType, requestType *r, char *buffer);
void sendSimpleResponse(requestType *r);
void sendFullResponse(requestType *r);
void sendSimpleError(requestType *r, int code, const char *message);
void sendFullError(requestType *r, int code, const char *message);