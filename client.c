#include "common.h"

int main(int argc, char const *argv[])
{
    int serverFd;
    struct sockaddr_in serverAddress;
    int addressLen = sizeof(serverAddress);

    if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Unable to create socket");
        return -1;
    }

    memset((char *)&serverAddress, 0, addressLen);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);

    if (connect(serverFd, (struct sockaddr*)&serverAddress, addressLen) < 0)
    {
        perror("Unable to connect to server");
        return -1;
    }

    doClientRequest(serverFd);

    return 0;
}

void doClientRequest(int serverFd)
{
    long valread;
    char buffer[MAX_BUFFER_SIZE] = {0};

    send(serverFd, "Here's your message\n", strlen("Here's your message\n"), 0);
    valread = read(serverFd, buffer, MAX_BUFFER_SIZE);

    printf("%s\n", buffer);

    close(serverFd);
}