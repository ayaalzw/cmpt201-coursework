#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void *thread_func(void *string) {
  printf("Thread ID: %d, Input: %s", gettid(), (char *)string);

  int *len = malloc(sizeof(int));
  len = (void *)strlen((char *)string);

  return (void *)len;
}
int main() {

  pthread_t thread_id;
  void *(*start_routine)(void *) = thread_func;
  char *string = "hi";

  pthread_create(&thread_id, NULL, start_routine, string);

  int *retval;
  if (!pthread_join(thread_id, (void **)&retval)) {
    perror("pthread_join");
  }

  free(retval);

  return 0;
}
