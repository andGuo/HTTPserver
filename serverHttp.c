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
    char *pattern = "^([A-Z]+)[[:blank:]]+(http?://[[:graph:]]*|/[[:graph:]]*)[[:blank:]]*(HTTP/[0-9][.][0-9])?.*\r?\n";
    char errBuffer[MAX_STRING];
    int rt;
    
    errorCheck(regcomp(&regex, pattern, REG_EXTENDED), "regcomp error");

    if ((rt = regexec(&regex, clientReqBuf, nmatch, pmatch, 0)) != 0)
    {
        regerror(rt, &regex, errBuffer, MAX_STRING);
        printf("%s\n", errBuffer);
        sendSimpleError(&req, 400, "Bad Request");
        return EXIT_FAILURE;
    }

    regfree(&regex);

    memcpy(req.method, &clientReqBuf[pmatch[1].rm_so], pmatch[1].rm_eo - pmatch[1].rm_so);
    memcpy(req.uri, &clientReqBuf[pmatch[2].rm_so], pmatch[2].rm_eo - pmatch[2].rm_so);
    memcpy(req.httpVer, &clientReqBuf[pmatch[3].rm_so], pmatch[3].rm_eo - pmatch[3].rm_so);

    printf("Regex parsed strings: %s,%s,%s\n", req.method, req.uri, req.httpVer);
    reply(&req);

    return 0;
}

void reply(requestType *r)
{
    if (r->httpVer[0] == '\0') //NULL character indicates regex pattern did not find version number
    {
        sendSimpleResponse(r); //send a http 0.9 response
    }
    else
    {
        sendFullResponse(r); //send a http 1.0 response
    }
}

void sendSimpleResponse(requestType *r)
{
    if (strcmp(r->method, REQUEST[get]) != 0) //Simple responses can only be GET methods
    {
        sendSimpleError(r, 501, "Not Implemented");
        return;
    }

    char buffer[MAX_BUFFER_SIZE] = {0};
    //Pretend I look up an HTML file here
    snprintf(buffer, sizeof(buffer), "%s\r\n", "<html>\r\n<body>\r\n\r\n<h1>My http0.9 response</h1>\r\n\r\n<p>Hello.</p>\r\n\r\n</body>\r\n</html>\r\n");
    send(r->socket, buffer, strlen(buffer), 0);
}

void sendSimpleError(requestType *r, int statusCode, const char *reason)
{
    char response[MAX_BUFFER_SIZE];

    snprintf(response, sizeof(response), "%d %s\r\n", statusCode, reason);

    errorCheck(send(r->socket, response, sizeof(response), 0), "Unable to send simple error"); 
}

void sendFullResponse(requestType *r)
{
    char buffer[MAX_BUFFER_SIZE] = {0};
    headerType header;

    char date[MAX_STRING];
    char serverHost[MAX_STRING];

    if (gethostname(serverHost, MAX_STRING) != 0){
        perror("Unable to get host name");
    }

    if (genTime(date) == 0){
        perror("Time greater than maxsize");
    }

    header.rq = r;
    header.strDate = date;
    header.serverName = serverHost;
    header.outBuffer = buffer;

    if (strcmp(r->method, REQUEST[get]) == 0)
    {
        //Pretend I look up a file here
        header.status = 200;
        header.reasonPhrase = "ok";
        header.contentType = "text/html";
        createHeader(&header);
        //'Append' file data
        strncat(buffer, "<html>\r\n<body>\r\n\r\n<h1>My http1.0 response</h1>\r\n\r\n<p>Hello.</p>\r\n\r\n</body>\r\n</html>\r\n", sizeof(buffer)-strlen(buffer)-1);
    }
    else if (strcmp(r->method, REQUEST[head]) == 0)
    {
        //Pretend I look up an HTML file here
        header.status = 200;
        header.reasonPhrase = "ok";
        header.contentType = "text/html";
        createHeader(&header);
    }
    else if (strcmp(r->method, REQUEST[post]) == 0)
    {
        sendFullError(&header, 204, "No content");
        strncat(buffer, "<html>\r\n<body>\r\n\r\n<h1>Under Construction</h1>\r\n\r\n<p>Thanks for the request but, we don't take POSTs yet :-)</p>\r\n\r\n</body>\r\n</html>\r\n", sizeof(buffer)-strlen(buffer)-1);
    }
    else
    {
        sendFullError(&header, 501, "Not Implemented");
        strncat(buffer, "<title>Error #500 internal error</title>\r\n<h1>Error #500 internal error</h1>\r\n", sizeof(buffer)-strlen(buffer)-1);
    }

    errorCheck(send(r->socket, buffer, sizeof(buffer), 0), "Unable to send full response");
    printf("Reply sent...\n");
}

void createHeader(headerType *h)
{
    snprintf(h->outBuffer, MAX_BUFFER_SIZE, "%s %d %s\r\nDate: %s\r\nServer: %s\r\nContent-type: %s\r\n\r\n",
    h->rq->httpVer, h->status, h->reasonPhrase, h->strDate, h->serverName, h->contentType);
}

void sendFullError(headerType *h, int statusCode, char *reason)
{
    h->status = statusCode;
    h->reasonPhrase = reason;
    h->contentType = "text/html";
    createHeader(h);
}