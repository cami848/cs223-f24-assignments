/*----------------------------------------------
 * Author: Camille Lopez
 * Date:12/5/2024
 * Description: This program implements a
 * function memstats to print information
 * about the heap.
 ---------------------------------------------*/

#include "rand.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define ROUNDS 3
#define BUFFER 5
#define LOOP 10

struct chunk {
  int size;
  int used;
  struct chunk *next;
};

/**
 * Function that gets the memory statistics
 * @param freelist  Pointer to thread_data
 * @param buffer  Pointer to an array
 * @param len length of buffer array
 * @return void
 */
void memstats(struct chunk *freelist, void *buffer[], int len) {
  int total_blocks = 0;
  int free_blocks = 0;
  int used_blocks = 0;
  int memory_allocated = 0;
  int free_memory = 0;
  int used_memory = 0;
  int utilized_memory = 0;
  float underutilized_memory = 0;

  for (int i = 0; i < len; i++) {
    struct chunk *current = (struct chunk *)buffer[i] - 1;
    if (buffer[i] != NULL) {
      total_blocks++;
      used_blocks++;
      used_memory += current->size;
      utilized_memory += current->used;
      memory_allocated += current->size;
    }
  }
  struct chunk *current_node = freelist;
  underutilized_memory = 1 - (float)(utilized_memory) / (float)(used_memory);
  while (current_node) {
    free_blocks++;
    total_blocks++;
    free_memory += current_node->size;
    memory_allocated += current_node->size;
    current_node = current_node->next;
  }

  // Print the stats
  printf("Total blocks: %d ", total_blocks);
  printf(" Free blocks: %d ", free_blocks);
  printf(" Used blocks: %d \n", used_blocks);
  printf("Total memory allocated: %d ", memory_allocated);
  printf(" Free memory: %d ", free_memory);
  printf(" Used memory: %d \n", used_memory);
  printf("Underutilized memory: %.2f \n", underutilized_memory);
}

int main(int argc, char *argv[]) {

  printf("Starting test..\n");

  srand(100);

  double timer;
  struct timeval tstart, tend;

  gettimeofday(&tstart, NULL);

  void *buffer[BUFFER];
  for (int i = 0; i < BUFFER; i++) {
    buffer[i] = NULL;
  }

  void *init = sbrk(0);
  void *current;
  printf("The initial top of the heap is %p.\n", init);
  for (int j = 0; j < ROUNDS; j++) {
    printf("---------------\n%d\n", j);

    for (int i = 0; i < LOOP; i++) {
      int index = rand() % BUFFER;
      if (buffer[index] != NULL) {
        free(buffer[index]);
        buffer[index] = NULL;
        printf("Freeing index %d\n", index);
      } else {
        size_t size = (size_t)randExp(8, 4000);
        int *memory = NULL;
        memory = malloc(size);

        if (memory == NULL) {
          fprintf(stderr, "malloc failed\n");
          return (1);
        }
        *memory = 123;
        buffer[index] = memory;
        printf("Allocating %d bytes at index %d\n", (int)size, index);
      }
    }
    extern struct chunk *flist;
    current = sbrk(0);
    int allocated = current - init;
    init = current;

    printf("The new top of the heap is %p.\n", current);
    printf("Increased by %d (0x%x) bytes\n", allocated, allocated);
    memstats(flist, buffer, BUFFER);
  }

  for (int i = 0; i < BUFFER; i++) {
    free(buffer[i]);
  }
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec) / 1.e6;
  printf("Time is %g\n", timer);

  return 0;
}
