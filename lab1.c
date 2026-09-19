#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

  printf("Please enter some text: ");

  char *buff = NULL, *token, *saveptr;
  size_t size = 0;
  ssize_t nread = getline(&buff, &size, stdin);

  if (nread == -1) {
    perror("error!");
    exit(EXIT_FAILURE);
  }

  printf("\nTokens:\n");
  token = strtok_r(buff, " ", &saveptr);
  printf("%s\n", token);

  while (token != NULL) {
    token = strtok_r(NULL, " ", &saveptr);

    if (token != NULL) {
      printf("%s\n", token);
    }
  }

  free(buff);

  return 0;
}
