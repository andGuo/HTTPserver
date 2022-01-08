#include "common.h"

void connectServer(int *serverSok)
{
    struct addrinfo serverInfo, *res;
    int yes = 1;

    memset(&serverInfo, 0, sizeof(serverInfo));
    serverInfo.ai_family = AF_UNSPEC;
    serverInfo.ai_socktype = SOCK_STREAM;
    serverInfo.ai_flags = AI_PASSIVE;

    errorCheck(getaddrinfo(NULL, STR_PORT, &serverInfo, &res), "Unable to getaddrinfo");

    errorCheck(*serverSok = socket(res->ai_family, res->ai_socktype, res->ai_protocol), "Unable to create socket");

    //errorCheck(setsockopt(*serverSok, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes), "setsockopt");
    //Reuse open port

    if (bind(*serverSok, res->ai_addr, res->ai_addrlen) < 0)
    {
        close(*serverSok);
        perror("Unable to bind socket");
        exit(EXIT_FAILURE);
    }

    errorCheck(listen(*serverSok, MAX_BACKLOG), "Unable to listen on socket");
    printf("Waiting for connection...\n");
}

void acceptConnect(int serverSok, int *clientSok)
{
    struct sockaddr_storage address;
    socklen_t addressLen = sizeof(address);

    errorCheck(*clientSok = accept(serverSok, (struct sockaddr *)&address, &addressLen), 
    "Unable to accept connection");

    printf("...connected\n");
}

void connectClient(int *serverSok)
{
    struct sockaddr_in serverAddress;
    int addressLen = sizeof(serverAddress);

    errorCheck(*serverSok = socket(AF_INET, SOCK_STREAM, 0), "Unable to create socket");

    memset((char *)&serverAddress, 0, addressLen);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    errorCheck(connect(*serverSok, (struct sockaddr*)&serverAddress, addressLen), "Unable to connect to server");
}

//Thanks Jacob Sorber
int errorCheck(int rtnVal, const char *message)
{
    if (rtnVal < 0)
    {
        perror(message);
        exit(EXIT_FAILURE);
    }
    return rtnVal;
}