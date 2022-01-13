#include "common.h"

int handleRequest(char *buffer)
{
    regex_t regex;
    
    char *pattern = "^((GET[[:blank:]]+)|(HEAD[[:blank:]]+)|(POST[[:blank:]]+))";
    size_t nmatch = 5;
    regmatch_t pmatch[5];
    char errBuffer[MAX_STRING];
    int rt;
    char method[MAX_REQUEST] = {0};
    char uri[MAX_BUFFER_SIZE] = {0};
    char httpVer[MAX_REQUEST] = {0};

    errorCheck(regcomp(&regex, pattern, REG_EXTENDED), "regcomp error");

    if ((rt = regexec(&regex, buffer, nmatch, pmatch, 0)) != 0)
    {
        regerror(rt, &regex, errBuffer, MAX_STRING);
        printf("%s\n", errBuffer);
        //send back error or something
        return 0;
    }

    printf("%s", &buffer[pmatch[3].rm_eo - pmatch[3].rm_so]);
    memcpy(method, &buffer[pmatch[1].rm_so], pmatch[1].rm_eo - pmatch[1].rm_so - 1);
    printf("%d, %d\n", pmatch[1].rm_eo, pmatch[1].rm_so);
    printf("%s", method);
    

    regfree(&regex);

    return 0;
}

void sendResponse(int type, char *directory)
{
    
}

int createHeader()
{

}
