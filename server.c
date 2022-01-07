#include "common.h"


int main(int argc, char *argv[])
{
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0)
    {
        perror("Unable to create socket");
        return 0;
    }

    struct sockaddr_in address;

    memset((char *)&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);

    if (bind(serverFd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Unable to bind socket");
        return 0;
    }

    if (listen(serverFd, MAX_BACKLOG) < 0)
    {
        perror("Unable to listen on socket");
        return 0;
    }

}