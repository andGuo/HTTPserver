#include "common.h"

int main(int argc, char *argv[])
{
    int serverFd, *clientFd;

    pthread_t threadPool[POOL_NUM_THREADS];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    queueType taskQueue;
    initQueue(&taskQueue);

    threadArgType arg = {
        .mutex = &mutex,
        .taskq = &taskQueue
    };

    void* argPtr = &arg;

    setUpServer(&serverFd);

    for (int i = 0; i < POOL_NUM_THREADS; ++i)
    {
        errorCheck(pthread_create(&threadPool[i], NULL, threadRoutine, argPtr), "Thread creation error");
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
    sleep(5);
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
    threadArgType *res = (threadArgType*) arg;
    pthread_mutex_t *mutex = res->mutex;
    queueType *queue = res->taskq;
    int *clientFd, rtn;

    while(1)
    {
        pthread_mutex_lock(mutex);
        rtn = dequeueTask(queue, &clientFd);
        pthread_mutex_unlock(mutex);
        if (rtn == 0)
        {
            serveOneClient(clientFd);
        }
    }
}