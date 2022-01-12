#include "common.h"

int handleRequest(char *buffer)
{
    regex_t regex;
    char *pattern = "^(GET[:space:]*)";
    size_t     nmatch = 2;
    regmatch_t pmatch[2];
    char errBuffer[MAX_STRING];
    int rt;

    errorCheck(regcomp(&regex, pattern, REG_EXTENDED), "regcomp error");

    if ((rt = regexec(&regex, buffer, nmatch, pmatch, 0)) != 0)
    {
        regerror(rt, &regex, errBuffer, MAX_STRING);
        printf("%s\n", errBuffer);
        //send back error or something
        return 0;
    }

    printf("%s", &buffer[pmatch[1].rm_eo - pmatch[1].rm_so]);

    regfree(&regex);

    return 0;
}

void sendResponse(int val)
{
    
}

int createHeader()
{

}
