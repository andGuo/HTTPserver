#include "common.h"

int handleRequest(char *clientReqBuf, int clientFd)
{
    requestType req = {
        .method = {0},
        .uri = {0},
        .httpVer = {0},
        .socket = clientFd
    };
    
    regex_t regex;
    size_t nmatch = 5;
    regmatch_t pmatch[5];
    char *pattern = "([A-Za-z]+)[[:blank:]]+(http?://.*|/[[:graph:]]*)[[:blank:]]*(HTTP/[0-9][.][0-9])?";
    char errBuffer[MAX_STRING];
    int rt;
    
    errorCheck(regcomp(&regex, pattern, REG_EXTENDED), "regcomp error");

    if ((rt = regexec(&regex, clientReqBuf, nmatch, pmatch, 0)) != 0)
    {
        regerror(rt, &regex, errBuffer, MAX_STRING);
        printf("%s\n", errBuffer);
        //senderror
        return EXIT_FAILURE;
    }

    regfree(&regex);

    memcpy(req.method, &clientReqBuf[pmatch[1].rm_so], pmatch[1].rm_eo - pmatch[1].rm_so);
    memcpy(req.uri, &clientReqBuf[pmatch[2].rm_so], pmatch[2].rm_eo - pmatch[2].rm_so);
    memcpy(req.httpVer, &clientReqBuf[pmatch[3].rm_so], pmatch[3].rm_eo - pmatch[3].rm_so);

    printf("%s,%s,%s\n", req.method, req.uri, req.httpVer);
    sendResponse(&req);

    return 0;
}

void sendResponse(requestType *request)
{
    char buffer[MAX_BUFFER_SIZE] = {0};

    if (strcmp(request->method, REQUEST[get]) == 0)
    {

    }
    else if (strcmp(request->method, REQUEST[head]) == 0)
    {

    }
    else if (strcmp(request->method, REQUEST[post]) == 0)
    {

    }
    else
    {
        //sendError
    }
}

void createHeader(RequestEnum rType, requestType *request)
{

}
