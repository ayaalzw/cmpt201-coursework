
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main() {
  pid_t cpid = fork();

  if (cpid < 0) {

  } else if (cpid > 0) {
    printf("Output from parent\n");
  } else {
    printf("Output from child\n");
    execl("/user/bin/ls", "/usr/bin/ls", "-a", "-l", NULL);
  }

  printf("%d\n", getpid());

  return 0;
}
