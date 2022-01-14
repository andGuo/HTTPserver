#include "common.h"

int main(int argc, char *argv[])
{
    int serverFd, *clientFd;

    pthread_t threadPool[POOL_NUM_THREADS];
    queueType taskQueue;
    void* taskq = &taskQueue;

    setUpServer(&serverFd);

    for (int i = 0; i < POOL_NUM_THREADS; ++i)
    {
        errorCheck(pthread_create(&threadPool[i], NULL, threadRoutine, taskq), "Thread creation error");
    }

    while (1)
    {
        printf("Waiting for connection...\n");
        acceptConnect(serverFd, &clientFd);
        enqueueTask(&taskQueue, clientFd);
    }

    close(serverFd);

    return 0;
}

void serveOneClient(int *clientFd)
{
    //sleep(5);
    char buffer[MAX_BUFFER_SIZE] = {0};

    errorCheck(recv(*clientFd, buffer, MAX_BUFFER_SIZE, 0), "Unable to receive data");
    printf("%s", buffer);
    handleRequest(buffer, clientFd);

    close(*clientFd);
    free(clientFd);

    printf("Connection closed...\n");
}

void *threadRoutine(void *arg)
{
    int *clientFd;
    queueType *taskQueue = (queueType*) arg;

    while(1)
    {
        //Probably need a mutex on the task queue here
        if (dequeueTask(taskQueue, &clientFd) == 0)
        {
            serveOneClient(clientFd);
        }
    }
}