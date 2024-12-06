/*----------------------------------------------
 * Author: Camille Lopez
 * Date:12/5/2024
 * Description: This program implements our
 * own version of malloc and free based on the
 * assigned reading.
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
  int size;
  int used;
  struct chunk *next;
};
struct chunk *flist = NULL;

/**
 * Function that mallocs memory
 * @param size  size of memory
 * @return void
 */
void *malloc(size_t size) {
  // TODO: Implement malloc with a free list (See reading for details)
  if (size == 0) {
    return NULL;
  }
  struct chunk *next = flist;
  struct chunk *prev = NULL;
  while (next != NULL) {
    if (next->size >= size) {
      next->used = size;
      if (prev != NULL) {
        prev->next = next->next;
      } else {
        flist = next->next;
      }
      return (void *)(next + 1);
    } else {
      prev = next;
      next = next->next;
    }
  }
  void *memory = sbrk(size + sizeof(struct chunk));
  if (memory == (void *)-1) {
    return NULL;
  } else {
    struct chunk *cnk = (struct chunk *)memory;
    cnk->size = size;
    cnk->used = size;
    return (void *)(cnk + 1);
  }
}

/**
 * Function that puts freed memory in a freelist
 * @param memory  Pointer to memory we are freeing
 * @return void
 */
void free(void *memory) {
  // TODO: Implement malloc with a free list (See reading for details)
  if (memory != NULL) {
    struct chunk *cnk = (struct chunk *)((struct chunk *)memory - 1);
    cnk->next = flist;
    flist = cnk;
    flist->used = 0;
  }
  return;
}
