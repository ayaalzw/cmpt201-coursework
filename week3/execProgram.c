
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t cpid = fork();

  if (cpid < 0) {

  } else if (cpid > 0) {
    printf("Output from parent\n");
    exec(arg0, arg0,
  } else {
    printf("Output from child\n");
  }

  printf("%d\n", getpid());

  return 0;
}
