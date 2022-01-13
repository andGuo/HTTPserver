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
    char *pattern = "([A-Za-z]+)[[:blank:]]+(http?://[[:graph:]]*|/[[:graph:]]*)[[:blank:]]*(HTTP/[0-9][.][0-9])?.*\r?\n";
    char errBuffer[MAX_STRING];
    int rt;
    
    errorCheck(regcomp(&regex, pattern, REG_EXTENDED), "regcomp error");

    if ((rt = regexec(&regex, clientReqBuf, nmatch, pmatch, 0)) != 0)
    {
        regerror(rt, &regex, errBuffer, MAX_STRING);
        printf("%s\n", errBuffer);
        //sendSimpleError
        return EXIT_FAILURE;
    }

    regfree(&regex);

    memcpy(req.method, &clientReqBuf[pmatch[1].rm_so], pmatch[1].rm_eo - pmatch[1].rm_so);
    memcpy(req.uri, &clientReqBuf[pmatch[2].rm_so], pmatch[2].rm_eo - pmatch[2].rm_so);
    memcpy(req.httpVer, &clientReqBuf[pmatch[3].rm_so], pmatch[3].rm_eo - pmatch[3].rm_so);

    printf("%s,%s,%s\n", req.method, req.uri, req.httpVer);
    respond(&req);

    return 0;
}

void respond(requestType *r)
{
    if (r->httpVer[0] == '\0') //NULL character indicates regex pattern did not find version number
    {
        sendSimpleResponse(r);
    }
    else //should be an http 1.0 or http 1.1 request.
    {
        sendFullResponse(r);
    }
}

void sendSimpleResponse(requestType *r)
{
    if (strcmp(r->method, REQUEST[get]) != 0) //Simple responses can only be GET methods
    {
        //sendSimpleError
        return;
    }

    char buffer[MAX_BUFFER_SIZE] = {0};
    //Pretend I look up an HTML file here
    snprintf(buffer, sizeof(buffer), "%s\r\n", "<html>\r\n<body>\r\n\r\n<h1>My First Heading</h1>\r\n\r\n<p>My first paragraph.</p>\r\n\r\n</body>\r\n</html>\r\n");
    send(r->socket, buffer, strlen(buffer), 0);
}

void sendFullResponse(requestType *r)
{
    char buffer[MAX_BUFFER_SIZE] = {0};
    if (strcmp(r->method, REQUEST[get]) == 0)
    {
        
    }
    else if (strcmp(r->method, REQUEST[head]) == 0)
    {

    }
    else if (strcmp(r->method, REQUEST[post]) == 0)
    {

    }
    else
    {
        //sendFullError
    }
}

void createHeader(RequestEnum rType, requestType *r, char *buffer)
{
    
}

void sendSimpleError(requestType *r, int code, const char *message)
{

}

void sendFullError(requestType *r, int code, const char *message)
{

}