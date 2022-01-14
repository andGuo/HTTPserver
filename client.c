#include "common.h"
#define SIMPLE 0
#define FULL 1

int main(int argc, char const *argv[])
{
    int serverFd, flag;
    
    if (argc > 1 && strcmp(argv[1], "simple") == 0)
    {
        flag = SIMPLE;
    }
    else
    {
        flag = FULL;
    }
    
    connectClient(&serverFd);

    switch(flag)
    {
        case SIMPLE:
            sendSimpleRequest("/example.html", serverFd);
            break;
        case FULL:
            sendFullRequest("/example.html", serverFd);
            break;
        default:
            perror("No request sent");
            break;
    }
    
    handleResponse(serverFd);

    close(serverFd);

    return 0;
}