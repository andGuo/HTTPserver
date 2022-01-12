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
    
    sendSimpleRequest("/example.html", serverFd);
    handleResponse(serverFd);

    close(serverFd);
}