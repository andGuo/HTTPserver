#include "common.h"

int main(int argc, char const *argv[])
{
    int clientFd;
    struct sockaddr_in serverAddress;
    int addressLen = sizeof(serverAddress);

    if ((clientFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Unable to create socket");
        return -1;
    }

    memset((char *)&serverAddress, 0, addressLen);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    if (connect(clientFd, (struct sockaddr*)&serverAddress, addressLen) < 0)
    {
        perror("Unable to connect to server");
        return -1;
    }

    doClientRequest(clientFd);

    return 0;
}

void doClientRequest(int clientFd)
{
    long valread;
    char buffer[MAX_BUFFER_SIZE] = {0};

    send(clientFd, "Here's your message\n", strlen("Here's your message\n"), 0);
    valread = read(clientFd, buffer, MAX_BUFFER_SIZE);

    printf("%s\n", buffer);
    
    close(clientFd);
}