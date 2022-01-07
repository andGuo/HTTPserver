#include "common.h"

int main(int argc, char const *argv[])
{
    int serverFd;
    
    connectClient(&serverFd);

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