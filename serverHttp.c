#include "common.h"

int handleRequest(char *buffer)
{
    regex_t regex;
    size_t nmatch = 5;
    regmatch_t pmatch[5];
    char *pattern = "([A-Za-z]+)[[:blank:]]+(http?://.*|/[[:graph:]]*)[[:blank:]]*(HTTP/[0-9][.][0-9])?";
    char errBuffer[MAX_STRING];
    int rt;
    requestType req = {
        .method = {0},
        .uri = {0},
        .httpVer = {0}
    };
    

    errorCheck(regcomp(&regex, pattern, REG_EXTENDED), "regcomp error");

    if ((rt = regexec(&regex, buffer, nmatch, pmatch, 0)) != 0)
    {
        regerror(rt, &regex, errBuffer, MAX_STRING);
        printf("%s\n", errBuffer);
        //send back error or something
        return 0;
    }

    regfree(&regex);

    memcpy(req.method, &buffer[pmatch[1].rm_so], pmatch[1].rm_eo - pmatch[1].rm_so);
    memcpy(req.uri, &buffer[pmatch[2].rm_so], pmatch[2].rm_eo - pmatch[2].rm_so);
    memcpy(req.httpVer, &buffer[pmatch[3].rm_so], pmatch[3].rm_eo - pmatch[3].rm_so);

    printf("%s,%s,%s\n", req.method, req.uri, req.httpVer);

    return 0;
}

void sendResponse(requestType request)
{
    
}

int createHeader()
{

}
