#include "common.h"

void connectServer(int *serverSok)
{
    struct sockaddr_in address;
    int addressLen = sizeof(address);

    errorCheck(*serverSok = socket(AF_INET, SOCK_STREAM, 0), "Unable to create socket");

    memset((char *)&address, 0, addressLen);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);

    errorCheck(bind(*serverSok, (struct sockaddr *)&address, addressLen), "Unable to bind socket");

    errorCheck(listen(*serverSok, MAX_BACKLOG), "Unable to listen on socket");
}

void acceptConnect(int serverSok, int *clientSok)
{
    struct sockaddr_in address;
    socklen_t addressLen = sizeof(address);

    printf("Waiting for connection...\n");

    errorCheck(*clientSok = accept(serverSok, (struct sockaddr *)&address, &addressLen), 
    "Unable to accept connection");

    printf("...connected\n");
}

void connectClient()
{

}

//Thanks Jacob Sorber
int errorCheck(int rtnVal, const char *message)
{
    if (rtnVal < 0)
    {
        perror(message);
        exit(-1);
    }
    return rtnVal;
}