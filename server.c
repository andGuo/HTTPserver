#include "common.h"

int main(int argc, char *argv[])
{
    int serverFd, clientFd;
    
    connectServer(&serverFd);

    while (1)
    {
        acceptConnect(serverFd, &clientFd);
        serveOneClient(clientFd);
        close(clientFd);
    }

    close(serverFd);

    return 0;
}

void serveOneClient(int clientFd)
{
    char buffer[MAX_BUFFER_SIZE] = {0};
    long valread;
    char *hello = "HTTP/1.0 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\nHello world!";

    valread = read(clientFd, buffer, MAX_BUFFER_SIZE);
    printf("%s", buffer);
    printf("Sending reply...\n");
    write(clientFd, hello, strlen(hello));

    close(clientFd);
}