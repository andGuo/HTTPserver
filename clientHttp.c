#include "common.h"

void sendSimpleRequest(char *uri, int serverFd)
{
    char request[MAX_BUFFER_SIZE] = {0};

    snprintf(request, sizeof(request), "%s %s\r\n", "GET", uri);

    errorCheck(send(serverFd, request, sizeof(request), 0), "Unable to send data");
}

void sendFullRequest(char *uri, int serverFd, const char *requestStr)
{
    char request[MAX_BUFFER_SIZE] = {0};
    char userAgent[MAX_STRING] = {0};

    if (gethostname(userAgent, MAX_STRING) != 0)
    {
        perror("Unable to get agent name");
    }

    snprintf(request, sizeof(request), "%s %s HTTP/1.0\r\nUser-Agent: %s\r\n", requestStr, uri, userAgent);

    errorCheck(send(serverFd, request, sizeof(request), 0), "Unable to send data");
}

void handleResponse(int serverFd)
{
    char buffer[MAX_BUFFER_SIZE] = {0};

    errorCheck(read(serverFd, buffer, MAX_BUFFER_SIZE), "Unable to read data");

    printf("%s\n", buffer);
}
