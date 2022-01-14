#include <common.h>

void initQueue(queueType *q)
{
  q->head = NULL;
  q->tail = NULL;
}

void addTask(queueType *q, int *clientSocket)
{
  nodeType *currentNode;
  nodeType *newNode;

  currentNode = q->tail;

  newNode = malloc(sizeof(nodeType));
  newNode->next = NULL;
  newNode->clientFd = clientSocket;

  if (isEmpty(q) == NULL)
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

int pollFirst(queueType *q, int **clientSocket)
{
  nodeType *currentNode;
  nodeType *nextNode;

  if (isEmpty(q) == NULL)
  {
    return NULL;
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

  return 1;
}

int isEmpty(queueType *q)
{
  if (q->head == NULL && q->tail == NULL)
  {
    return NULL;
  }
  else
  {
    return 1;
  }
}

void cleanQueue(queueType *q)
{
  nodeType *currentNode;
  nodeType *nextNode;

  currentNode = q->head;

  while (currentNode != NULL)
  {
    nextNode = currentNode->next;
    cleanupSocket(currentNode->clientFd);
    free(currentNode);
    currentNode = nextNode;
  }

  q->head = NULL;
  q->tail = NULL;
}
