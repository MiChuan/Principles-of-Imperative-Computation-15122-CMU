/*
 * Extended interface of queues
 *
 * 15-122 Principles of Imperative Computation
 */
#include <stdlib.h>
#include <stdbool.h>
#include "lib/contracts.h"
#include "lib/xalloc.h"
#include "queue.h"


/**********************/
/*** Implementation ***/
/**********************/

typedef struct list_node list;
struct list_node {
  void *data;
  list *next;
};

typedef struct queue_header queue;
struct queue_header {
  list *front;
  list *back;
  size_t size;
};

//helper function for is_queue
bool is_inclusive_segment(list *start, list *end, int i, int length) {
  if (start == NULL) return false;
  if (length > i) return false; // check circle
  if (start == end && start->next == NULL && length == i) return true;
  length++;
  return is_inclusive_segment(start->next,  end, i, length);
}

//is_queue main function
bool is_queue(queue *Q) {
  int length = 1;
  return Q != NULL
         && ((Q->front == NULL && Q->size == 0) // check empty queue
         || (is_inclusive_segment(Q->front, Q->back, Q->size, length)));
}

//return the size of queue
size_t queue_size(queue *Q)
{
  REQUIRES(is_queue(Q));
  size_t res = Q->size;
  return res;
}

/* Other functions go there */
// generate a new queue
queue *queue_new()
{
  queue *Q = xmalloc(sizeof(queue));
  Q->front = NULL;
  Q->size = 0;
  ENSURES(is_queue(Q));
  ENSURES(queue_size(Q) == 0);
  return Q;
}

//enqueue an elem
void enq(queue *Q, void *x)
{
  REQUIRES(is_queue(Q));
  list *l = xmalloc(sizeof(list));
  l->data = x;
  l->next = NULL;
  if (Q->size == 0) {
    Q->front = l;
    Q->back = l;
    Q->size++;
  } else {
    Q->back->next = l;
    Q->back = l;
    Q->size++;
  }
  ENSURES(is_queue(Q));
}


//dequeue an elem
void *deq(queue *Q)
{
  REQUIRES(is_queue(Q) && queue_size(Q) > 0);
  void *elem = Q->front->data;
  list *tmp = Q->front->next;
  Q->size--;
  free(Q->front);
  Q->front = tmp;
  ENSURES(is_queue(Q));
  return elem;
}

//peek an elem
void *queue_peek(queue *Q, size_t i)
{
  REQUIRES(is_queue(Q));
  REQUIRES(i < queue_size(Q));
  if (Q->size == 0) {
    ENSURES(is_queue(Q));
    return NULL;
  }
  size_t count = 0;
  for (list *q = Q->front; q != NULL; q = q->next) {
    if (count == i) {
      ENSURES(is_queue(Q));
      return q->data;
    }
    count++;
  }
  ENSURES(is_queue(Q));
  return NULL;
}

// reverse a queue
void queue_reverse(queue *Q)
{
  REQUIRES(is_queue(Q));
  if (Q->size == 0) {
    ENSURES(is_queue(Q));
    return;
  }
  if (Q->size == 1) {
    ENSURES(is_queue(Q));
    return;
  }
  list *first = Q->front;
  list *curr = first->next;
  list *prev = first;
  if (Q->size == 2) {
    Q->back->next = Q->front;
    Q->front->next = NULL;
    Q->front = curr;
    Q->back = first;
    ENSURES(is_queue(Q));
    return;
  }
  while (curr != Q->back)
  {
    if (prev == Q->front) {
      prev->next = NULL;
    }
    list *tmp_curr = curr->next;
    curr->next = prev;
    prev = curr;
    curr = tmp_curr;
  }
  Q->back->next = prev;
  Q->front = Q->back;
  Q->back = first;
  ENSURES(is_queue(Q));
  return;
}

//check property return true on all elem of the queue
bool queue_all(queue *Q, check_property_fn *P)
{
  REQUIRES(is_queue(Q) && P != NULL);
  if (Q->size == 0) return true;
  for (list *q = Q->front; q != NULL; q = q->next) {
    if (!(*P)(q->data)) return false;
  }
  return true;
}

//this is the queue interator
void *queue_iterate(queue *Q, void *base, iterate_fn *F)
{
  REQUIRES(is_queue(Q) && F != NULL);
  if (Q->size == 0) return base;
  void *accum = base;
  for (list *q = Q->front; q != NULL; q = q->next) {
    accum = (*F)(accum, q->data);
  }
  return accum;
}

//this is the queue free function used by client 
void queue_free(queue *Q, free_fn *F)
{
  REQUIRES(is_queue(Q));
  if(Q->size == 0) {
    free(Q);
    return;
  }
  if (F != NULL) {
    for (list *q = Q->front; q != NULL; q = q->next) {
      (*F)(q->data);
    }
  }
  // free nodes
  list *curr = Q->front->next;
  list *prev = Q->front;
  if (Q->size == 1) {
    free(prev);
    free(Q);
    return;
  }
  while (curr != NULL) {
    free(prev);
    prev = curr;
    curr = curr->next;
  }
  free(prev);
  free(Q);
  return;
}




