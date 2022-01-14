#include "common.h"

//Thanks Jacob Sorber
int errorCheck(int rtnVal, const char *message)
{
    if (rtnVal < 0)
    {
        perror(message);
        exit(EXIT_FAILURE);
    }
    return rtnVal;
}

//Thanks Beej
void *getInAddr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    //else must be AF_INET6
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int genTime(char *dateStr)
{
    time_t now = time(0);
    struct tm time = *gmtime(&now);
    return strftime(dateStr, MAX_STRING, "%a, %d %b %Y %H:%M:%S %Z", &time);
}