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
#include <time.h>
#include <pthread.h>

#define POOL_NUM_THREADS 4
#define PORT 8080
#define STR_PORT "8080"
#define MAX_BACKLOG 5
#define MAX_BUFFER_SIZE 4096
#define MAX_STRING 256
#define MAX_REQUEST 16

typedef enum {get, head, post, error} RequestEnum;
static const char *REQUEST[] = {"GET", "HEAD", "POST"};

typedef struct {
    char method[MAX_REQUEST];
    char uri[MAX_BUFFER_SIZE];
    char httpVer[MAX_REQUEST];
    int socket;
} requestType;

typedef struct {
    requestType *rq;
    int status;
    char *reasonPhrase;
    char *strDate;
    char *serverName;
    char *contentType;
    char *outBuffer;
} headerType;

typedef struct {
  nodeType *head;
  nodeType *tail;
} queueType;

typedef struct node{
    struct node *next;
    int *clientFd;
} nodeType;

/* Function forward references */

//server.c
void serveOneClient(int clientFd);

//client.c
void doClientRequest(int serverFd);

//util.c
int errorCheck(int rtnVal, const char *message);
void *getInAddr(struct sockaddr *sa);
int genTime(char *dateStr);

//connect.c
void setUpServer(int *serverSok);
void acceptConnect(int serverSok, int *clientSok);
void connectClient(int *serverSok);

//clientHttp.c
void sendSimpleRequest(char *uri, int serverFd);
void sendFullRequest(char *uri, int serverFd, const char *requestStr);
void handleResponse(int serverFd);

//serverHttp.c
int handleRequest(char *buffer, int clientFd);
void reply(requestType *r);
void createHeader(headerType *h);
void sendSimpleResponse(requestType *r);
void sendFullResponse(requestType *r);
void sendSimpleError(requestType *r, int statusCode, const char *reason);
void sendFullError(headerType *r, int statusCode, char *reason);

//queue.c
void initQueue(queueType *q);
void addTask(queueType *q, int *clientSocket);
int pollFirst(queueType *q, int **clientSocket);
int isEmpty(queueType *q);
void cleanQueue(queueType *q);