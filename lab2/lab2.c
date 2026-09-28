#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {

  char *buff = NULL;
  size_t size = 0;
  int wstatus = 0;

  printf("Enter programs to run.\n");
  ssize_t input = getline(&buff, &size, stdin);

  if (input == -1) {
    perror("error!\n");
    exit(EXIT_FAILURE);
  }

  buff[input - 1] = '\0';

  pid_t cpid = fork();

  if (cpid < 0) {
    perror("fork error\n");
    exit(EXIT_FAILURE);
  } else if (cpid > 0) {
    if (waitpid(cpid, &wstatus, 0) == -1) {
      perror("waitpid error\n");
      exit(EXIT_FAILURE);
    } else {
      execlp("./lab2.out", "lab2.out", (char *)NULL);

      perror("Exec failure\n");
      exit(EXIT_FAILURE);
    }
  } else {
    execlp(buff, buff, (char *)NULL);

    perror("Exec failure\n");
    exit(EXIT_FAILURE);
  }

  free(buff);

  return 0;
}
