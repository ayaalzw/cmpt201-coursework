#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int cnt = 0;

void *thread_func(void *count) {

  for (int i = 0; i < 10000000; i++) {
    cnt++;
  }

  return NULL;
}
void *thread_func2(void *count) {

  for (int i = 0; i < 10000000; i++) {
    cnt--;
  }

  return NULL;
}
int main() {

  pthread_t thread_id, thread_id2;
  void *(*start_routine)(void *) = thread_func;
  void *(*start_routine2)(void *) = thread_func2;
  char *string = "hi";

  pthread_create(&thread_id, NULL, start_routine, string);
  pthread_create(&thread_id2, NULL, start_routine2, string);

  if (pthread_join(thread_id, NULL)) {
    perror("pthread_join");
  }
  if (pthread_join(thread_id2, NULL)) {
    perror("pthread_join");
  }

  printf("%d\n", cnt);

  return 0;
}
