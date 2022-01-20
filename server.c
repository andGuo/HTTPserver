#include "common.h"

int main(int argc, char *argv[])
{
    int serverFd, *clientFd;

    int numThreads = DEFAULT_NUM_THREADS;
    int numResponses = DEFAULT_CONNECTIONS_DONE;

    if (argc > 2)
    {
        numThreads = atoi(argv[1]);
        numResponses = atoi(argv[2]);
    }
    else if (argc > 1)
    {
        numThreads = atoi(argv[1]);
    }

    printf("\nStarting server with %d threads and shutdown after responding to %d requests\n\n", numThreads, numResponses);

    pthread_t threadPool[numThreads];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t conditionVar = PTHREAD_COND_INITIALIZER;
    queueType taskQueue;
    initQueue(&taskQueue);

    threadArgType arg = {
        .mutex = &mutex,
        .taskq = &taskQueue,
        .conVar = &conditionVar,
        .keepRunning = 1};

    void *argPtr = &arg;

    setUpServer(&serverFd);

    for (int i = 0; i < numThreads; ++i)
    {
        errorCheck(pthread_create(&threadPool[i], NULL, threadRoutine, argPtr), "Thread creation error");
    }
    
    int i = 0;
    while (i < numResponses)
    {
        printf("Waiting for connection %d...\n", ++i);
        acceptConnect(serverFd, &clientFd);
        pthread_mutex_lock(&mutex);
        enqueueTask(&taskQueue, clientFd);
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&conditionVar);
    }

    while (isEmpty(&taskQueue) == 0) //Wait for task queue to empty
    {
        usleep(500000);
    }

    //Clean up runtime
    arg.keepRunning = 0;
    pthread_cond_broadcast(&conditionVar);

    for (int i = 0; i < numThreads; ++i)
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
    sleep(1); //artificial delay
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
    threadArgType *res = (threadArgType *)arg;
    pthread_mutex_t *mutex = res->mutex;
    queueType *queue = res->taskq;
    pthread_cond_t *conditionVar = res->conVar;

    while (res->keepRunning != 0)
    {
        pthread_mutex_lock(mutex);
        rtn = dequeueTask(queue, &clientFd);
        while (rtn != 0)
        {
            if (res->keepRunning == 0)
            {
                break;
            }
            pthread_cond_wait(conditionVar, mutex); //unlocks, waits, and locks
            rtn = dequeueTask(queue, &clientFd);
        }

        pthread_mutex_unlock(mutex); //unlocked regardless of whether thread waited or not
        if (rtn == 0)
        {
            serveOneClient(clientFd);
        }
    }
    return (0);
}