#define _POSIX_C_SOURCE 200809L
#include <stddef.h>
#include <stdio.h>
#include <string.h>

void read_user_input(char **buff, size_t *size) { getline(buff, size, stdin); }

void display_five_inputs(char **arr) {
  for (int i = 0; i < 5; i++) {
    if (arr[i] != NULL) {
      printf("%s", arr[i]);
    }
  }
}

int main() {
  size_t size = 0;
  char *arr[5] = {NULL};
  int i = 0;
  while (1) {
    printf("Enter input: ");

    read_user_input(&arr[i % 5], &size);

    if (strcmp(arr[i % 5], "print\n") == 0) {
      display_five_inputs(arr);
    }

    i++;
  }
}
