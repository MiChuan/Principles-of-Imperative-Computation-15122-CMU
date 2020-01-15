#include <stdlib.h>
#include <stdbool.h>
#include "lib/contracts.h"
#include "lib/xalloc.h"
#include "queue.h"

int main()
{
  queue_t Q0 = queue_new();
  int *num1 = xmalloc(sizeof(int));
  *num1 = 8;
  int *num2 = xmalloc(sizeof(int));
  *num2 = 5;
  int *num3 = xmalloc(sizeof(int));
  *num3 = 15;
  enq(Q0, (void*)num1);
  ASSERT(queue_size(Q0) == 1);
  enq(Q0, (void*)num2);
  ASSERT(queue_size(Q0) == 2);
  enq(Q0, (void*)num3);
  ASSERT(queue_size(Q0) == 3);
  ASSERT(*(int*)queue_peek(Q0, 0) == 8);
  ASSERT(*(int*)queue_peek(Q0, 1) == 5);
  ASSERT(*(int*)queue_peek(Q0, 2) == 15);
  ASSERT(queue_size(Q0) == 3);
  ASSERT(*(int*)deq(Q0) == 8);
  ASSERT(*(int*)deq(Q0) == 5);
  ASSERT(*(int*)deq(Q0) == 15);
  ASSERT(queue_size(Q0) == 0);
  queue_t Q1 = queue_new();
  int *num4 = xmalloc(sizeof(int));
  *num4 = 8;
  int *num5 = xmalloc(sizeof(int));
  *num5 = 5;
  int *num6 = xmalloc(sizeof(int));
  *num6 = 15;
  enq(Q1, (void*)num4);
  enq(Q1, (void*)num5);
  enq(Q1, (void*)num6);
  ASSERT(*(int*)queue_peek(Q1, 0) == 8);
  ASSERT(*(int*)queue_peek(Q1, 1) == 5);
  ASSERT(*(int*)queue_peek(Q1, 2) == 15);
  queue_reverse(Q1);
  ASSERT(*(int*)queue_peek(Q1, 0) == 15);
  ASSERT(*(int*)queue_peek(Q1, 1) == 5);
  ASSERT(*(int*)queue_peek(Q1, 2) == 8);
  ASSERT(*(int*)deq(Q1) == 15);
  ASSERT(*(int*)deq(Q1) == 5);
  ASSERT(*(int*)deq(Q1) == 8);
  ASSERT(queue_size(Q1) == 0);
  queue_t Q2 = queue_new();
  int *num7 = xmalloc(sizeof(int));
  *num7 = 8;
  int *num8 = xmalloc(sizeof(int));
  *num8 = 6;
  int *num9 = xmalloc(sizeof(int));
  *num9 = 10;
  enq(Q2, (void*)num7);
  enq(Q2, (void*)num8);
  enq(Q2, (void*)num9);
  deq(Q2);
  deq(Q2);
  deq(Q2);
  ASSERT(queue_size(Q2) == 0);
  int *num10 = xmalloc(sizeof(int));
  *num10 = 1;
  int *num11 = xmalloc(sizeof(int));
  *num11 = 1;
  int *num12 = xmalloc(sizeof(int));
  *num12 = 10;
  enq(Q2, (void*)num10);
  enq(Q2, (void*)num11);
  enq(Q2, (void*)num12);
  deq(Q2);
  deq(Q2);
  deq(Q2);
  ASSERT(queue_size(Q2) == 0);
  int *num14 = xmalloc(sizeof(int));
  *num14 = 1;
  int *num15 = xmalloc(sizeof(int));
  *num15 = 10;
  enq(Q2, NULL);
  enq(Q2, (void*)num14);
  enq(Q2, (void*)num15);
  deq(Q2);
  deq(Q2);
  deq(Q2);
  ASSERT(queue_size(Q2) == 0);
  int *num16 = xmalloc(sizeof(int));
  *num16 = 1;
  int *num17 = xmalloc(sizeof(int));
  *num17 = 10;
  int *num18 = xmalloc(sizeof(int));
  *num18 = - 10;
  enq(Q2, (void*)num16);
  enq(Q2, (void*)num17);
  enq(Q2, (void*)num18);
  deq(Q2);
  deq(Q2);
  deq(Q2);
  ASSERT(queue_size(Q2) == 0);
  enq(Q2, (void*)num10);
  enq(Q2, (void*)num11);
  enq(Q2, (void*)num12);
  enq(Q2, NULL);
  deq(Q2);
  deq(Q2);
  deq(Q2);
  deq(Q2);
  ASSERT(queue_size(Q2) == 0);
  enq(Q2, (void*)num16); // 1
  enq(Q2, (void*)num17); // 10
  enq(Q2, (void*)num18); // -10
  deq(Q2);
  deq(Q2);
  deq(Q2);
  ASSERT(queue_size(Q2) == 0);
  enq(Q2, NULL);
  enq(Q2, (void*)num14);
  enq(Q2, (void*)num15);
  deq(Q2);
  deq(Q2);
  deq(Q2);
  ASSERT(queue_size(Q2) == 0);
  enq(Q2, NULL);
  enq(Q2, (void*)num14);
  enq(Q2, (void*)num15);
  deq(Q2);
  deq(Q2);
  deq(Q2);
  int *num20 = xmalloc(sizeof(int));
  *num15 = 0;
  int *num21 = xmalloc(sizeof(int));
  *num21 = 9;
  int *num22 = xmalloc(sizeof(int));
  *num22 = 20;
  int *num23 = xmalloc(sizeof(int));
  *num23 = 40;
  int *num24 = xmalloc(sizeof(int));
  *num24 = -1;
  queue_t Q5 = queue_new();
  enq(Q5, (void*)num20);
  enq(Q5, (void*)num21);
  ASSERT(*(int*)queue_peek(Q5, 0) == 0);
  ASSERT(*(int*)queue_peek(Q5, 1) == 9);
  queue_reverse(Q5);
  ASSERT(*(int*)queue_peek(Q5, 0) == 9);
  ASSERT(*(int*)queue_peek(Q5, 1) == 0);
  ASSERT(queue_size(Q5) == 2);
  printf("all tests passed!");
  return 0;
}





