#include "common.h"

int main(int argc, char *argv[])
{
    int serverFd, clientFd;
    
    setUpServer(&serverFd);

    while (1)
    {
        printf("Waiting for connection...\n");
        acceptConnect(serverFd, &clientFd);
        serveOneClient(clientFd);
    }

    close(serverFd);

    return 0;
}

void serveOneClient(int clientFd)
{
    char buffer[MAX_BUFFER_SIZE] = {0};

    errorCheck(recv(clientFd, buffer, MAX_BUFFER_SIZE, 0), "Unable to receive data");
    printf("%s", buffer);
    handleRequest(buffer, clientFd);

    close(clientFd);
    printf("Connection closed...\n");
}