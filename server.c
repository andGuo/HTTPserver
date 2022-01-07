#include "common.h"

int main(int argc, char *argv[])
{
    int serverFd, clientFd;
    struct sockaddr_in address;
    int addressLen = sizeof(address);
    char buffer[MAX_BUFFER_SIZE] = {0};

    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Unable to create socket");
        return -1;
    }

    memset((char *)&address, 0, addressLen);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);

    if (bind(serverFd, (struct sockaddr *)&address, addressLen) < 0)
    {
        perror("Unable to bind socket");
        return -1;
    }

    if (listen(serverFd, MAX_BACKLOG) < 0)
    {
        perror("Unable to listen on socket");
        return -1;
    }

    while (1)
    {
        printf("Waiting for connection...\n");
        if ((clientFd = accept(serverFd, (struct sockaddr *)&address, (socklen_t*)&addressLen)) < 0)
        {
            perror("Unable to accept connection");
            return -1;
        }
        printf("...connected\n");

        serveOneClient(clientFd, buffer);
        
        close(clientFd);
    }

    close(serverFd);

    return 0;
}

void serveOneClient(int clientFd, char *buffer)
{
    long valread;
    char *hello = "HTTP/1.0 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

    valread = read(clientFd, buffer, MAX_BUFFER_SIZE);
    printf("%s", buffer);
    printf("Sending reply...\n");
    write(clientFd, hello, strlen(hello));
}