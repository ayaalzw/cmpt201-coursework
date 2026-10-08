#define _DEFAULT_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *inc_heap_size(int size);
struct header *initialize_first_block(char *first_block_ptr);
struct header *initialize_second_block(char *second_block_ptr);
void print_out(char *format, void *data, size_t data_size);

struct header {
  uint64_t size;
  struct header *next;
};

int main() {

  char *ptr = inc_heap_size(256);

  struct header *block1 = initialize_first_block(ptr);
  struct header *block2 = initialize_second_block(ptr + block1->size);

  print_out("first block:       %p\n", &block1, sizeof(&block1));
  print_out("second block:      %p\n", &block2, sizeof(&block2));
  print_out("first block size:  %d\n", &block1->size, sizeof(&block1->size));
  print_out("first block next:  %p\n", &block1->next, sizeof(&block1->next));
  print_out("second block size: %d\n", &block2->size, sizeof(&block2->size));
  print_out("second block next: %p\n", &block2->next, sizeof(&block2->next));

  for (int i = 0; i < 128 - sizeof(struct header); i++) {
    print_out("%hhu\n", ((char *)block1 + sizeof(struct header) + i), sizeof(char));
  }

  for (int i = 0; i < 128 - sizeof(struct header); i++) {
    print_out("%hhu\n", ((char *)block2 + sizeof(struct header) + i), sizeof(char));
  }

  return 0;
}

char *inc_heap_size(int size) {

  char *heap_start = sbrk(0);

  if (sbrk(size) == (void *)-1) {
    perror("sbrk:");
    exit(EXIT_FAILURE);
  }

  return heap_start;
}

struct header *initialize_first_block(char *first_block_ptr) {
  struct header *my_block = (struct header *)first_block_ptr;
  my_block->next = NULL;
  my_block->size = 128;
  memset(first_block_ptr + sizeof(struct header), 0, my_block->size - sizeof(struct header));
  return my_block;
}

struct header *initialize_second_block(char *second_block_ptr) {
  struct header *my_block = (struct header *)second_block_ptr;
  my_block->next = (struct header *)(second_block_ptr - 128);
  my_block->size = 128;
  memset(second_block_ptr + sizeof(struct header), 1, my_block->size - sizeof(struct header));
  return my_block;
}

void print_out(char *format, void *data, size_t data_size) {
  int BUF_SIZE = 100;
  char buf[BUF_SIZE];
  ssize_t len = snprintf(buf, BUF_SIZE, format,
                         data_size == sizeof(uint64_t) ? *(uint64_t *)data : *(void **)data);
  if (len < 0) {
    perror("snprintf");
    exit(EXIT_FAILURE);
  }
  write(STDOUT_FILENO, buf, len);
}
