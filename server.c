#include "common.h"

int main(int argc, char *argv[])
{
    int serverFd, *clientFd;

    pthread_t threadPool[POOL_NUM_THREADS];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t conditionVar = PTHREAD_COND_INITIALIZER;
    queueType taskQueue;
    initQueue(&taskQueue);

    threadArgType arg = {
        .mutex = &mutex,
        .taskq = &taskQueue,
        .conVar = &conditionVar,
        .keepRunning = 1
    };

    void* argPtr = &arg;

    setUpServer(&serverFd);

    for (int i = 0; i < POOL_NUM_THREADS; ++i)
    {
        errorCheck(pthread_create(&threadPool[i], NULL, threadRoutine, argPtr), "Thread creation error");
    }
    int i = 0;
    while (i < 100)
    {
        printf("Waiting for connection %d...\n", i++ + 1);
        acceptConnect(serverFd, &clientFd);
        pthread_mutex_lock(&mutex);
        enqueueTask(&taskQueue, clientFd);
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&conditionVar);
    }

    //Clean up runtime
    arg.keepRunning = 0;
    pthread_cond_broadcast(&conditionVar);

    for (int i = 0; i < POOL_NUM_THREADS; ++i)
    {
        errorCheck(pthread_join(threadPool[i], NULL), "Thread join error");
    }

    close(serverFd);
    cleanQueue(&taskQueue);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&conditionVar);

    return 0;
}

void serveOneClient(int *clientFd)
{
    char buffer[MAX_BUFFER_SIZE] = {0};

    errorCheck(recv(*clientFd, buffer, MAX_BUFFER_SIZE, 0), "Unable to receive data");
    handleRequest(buffer, clientFd);

    close(*clientFd);
    free(clientFd);

    printf("Connection closed...\n");
}

void *threadRoutine(void *arg)
{
    int *clientFd, rtn;
    threadArgType *res = (threadArgType*) arg;
    pthread_mutex_t *mutex = res->mutex;
    queueType *queue = res->taskq;
    pthread_cond_t *conditionVar = res->conVar;

    while(res->keepRunning)
    {
        pthread_mutex_lock(mutex);
        rtn = dequeueTask(queue, &clientFd);
        if (rtn != 0)
        {
            pthread_cond_wait(conditionVar, mutex);
            rtn = dequeueTask(queue, &clientFd);
        }
        
        pthread_mutex_unlock(mutex);
        if (rtn == 0)
        {
            serveOneClient(clientFd);
        }
    }
}