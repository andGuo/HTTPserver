#include "common.h"

void sendSimpleRequest(const char* uri, int serverFd)
{
    char request[MAX_BUFFER_SIZE];

    snprintf(request, sizeof(request), "%s%s\r\n", "GET ", document);

    errorCheck(send(serverFd, request, sizeof(request), 0), "Unable to send data"); 
}


void sendFullRequest()
{

}


void handleResponse(int serverFd)
{
    char buffer[MAX_BUFFER_SIZE] = {0};

    errorCheck(read(serverFd, buffer, MAX_BUFFER_SIZE), "Unable to read data");

    printf("%s\n", buffer);
}
