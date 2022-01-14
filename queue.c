#include "common.h"

void initQueue(queueType *q)
{
  q->head = NULL;
  q->tail = NULL;
}

void enqueueTask(queueType *q, int *clientSocket)
{
  nodeType *currentNode;
  nodeType *newNode;

  currentNode = q->tail;

  newNode = malloc(sizeof(nodeType));
  newNode->next = NULL;
  newNode->clientFd = clientSocket;

  if (q->head == NULL && q->tail == NULL)
  {
    q->head = newNode;
    q->tail = newNode;
  }
  else
  {
    q->tail = newNode;
    currentNode->next = newNode;
  }
}

int dequeueTask(queueType *q, int **clientSocket)
{
  nodeType *currentNode;
  nodeType *nextNode;

  if (q->head == NULL && q->tail == NULL)
  {
    return -1;
  }

  currentNode = q->head;
  nextNode = q->head->next;

  *clientSocket = currentNode->clientFd;

  free(currentNode); // Remove first (head)

  if (nextNode == NULL) // is now an empty list
  {
    q->head = NULL;
    q->tail = NULL;
  }
  else
  {
    q->head = nextNode;
  }

  return 0;
}

void cleanQueue(queueType *q)
{
  nodeType *currentNode;
  nodeType *nextNode;

  currentNode = q->head;

  while (currentNode != NULL)
  {
    nextNode = currentNode->next;
    free(currentNode->clientFd);
    free(currentNode);
    currentNode = nextNode;
  }

  q->head = NULL;
  q->tail = NULL;
}
