#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct header {
  uint64_t size;
  struct header *next;
  int id;
};

void initialize_block(struct header *block, uint64_t size, struct header *next, int id) {
  block->size = size;
  block->next = next;
  block->id = id;
}

int find_first_fit(struct header *free_list_ptr, uint64_t size) {
  // TODO: Implement first fit
  struct header *current_block = free_list_ptr;
  while (current_block != NULL) {
    if (current_block->size >= size) {
      return current_block->id;
    }
    current_block = current_block->next;
  }
  return -1;
}

int find_best_fit(struct header *free_list_ptr, uint64_t size) {
  int best_fit_id = -1;
  // TODO: Implement best fit
  uint64_t best_size = UINT64_MAX;
  struct header *current_block = free_list_ptr;
  while (current_block != NULL) {
    if (current_block->size == size) {
      return current_block->id;
    }
    if (current_block->size > size && best_size > current_block->size) {
      best_fit_id = current_block->id;
      best_size = current_block->size;
    }
    current_block = current_block->next;
  }
  return best_fit_id;
}

int find_worst_fit(struct header *free_list_ptr, uint64_t size) {
  int worst_fit_id = -1;
  // TODO: Implement worst fit
  uint64_t worst_size = 0;
  struct header *current_block = free_list_ptr;
  while (current_block != NULL) {
    if (current_block->size == size) {
      return current_block->id;
    }
    if (current_block->size > size && worst_size < current_block->size) {
      worst_fit_id = current_block->id;
      worst_size = current_block->size;
    }
    current_block = current_block->next;
  }

  return worst_fit_id;
}

int main(void) {

  struct header *free_block1 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block2 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block3 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block4 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block5 = (struct header *)malloc(sizeof(struct header));

  initialize_block(free_block1, 6, free_block2, 1);
  initialize_block(free_block2, 12, free_block3, 2);
  initialize_block(free_block3, 24, free_block4, 3);
  initialize_block(free_block4, 8, free_block5, 4);
  initialize_block(free_block5, 4, NULL, 5);

  struct header *free_list_ptr = free_block1;

  int first_fit_id = find_first_fit(free_list_ptr, 7);
  int best_fit_id = find_best_fit(free_list_ptr, 7);
  int worst_fit_id = find_worst_fit(free_list_ptr, 7);

  // TODO: Print out the IDs
  if (first_fit_id == -1) {
    printf("First-Fit algorithm: There is no block big enough for this size\n");
  } else {
    printf("The ID for First-Fit algorithm is: %d\n", first_fit_id);
  }
  if (best_fit_id == -1) {
    printf("Best-Fit algorithm: There is no block big enough for this size\n");
  } else {
    printf("The ID for Best-Fit algorithm is: %d\n", best_fit_id);
  }
  if (worst_fit_id == -1) {
    printf("Worst-Fit algorithm: There is no block big enough for this size\n");
  } else {
    printf("The ID for Worst-Fit algorithm is: %d\n", worst_fit_id);
  }

  free(free_block1);
  free(free_block2);
  free(free_block3);
  free(free_block4);
  free(free_block5);

  return 0;
}

/*
 * void coalesce(struct header block) {
 *
 *  struct header *next = *(block->next);
 *
 * }
 * */
