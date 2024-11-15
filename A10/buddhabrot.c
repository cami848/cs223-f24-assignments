/*----------------------------------------------
 * Author: Camille Lopez
 * Date:11/15/2024
 * Description: This program outputs a PPM image
 *  of the Buddhabrot using multiple threads.
 ---------------------------------------------*/

#include "read_ppm.h"
#include "write_ppm.h"
#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

struct thread_data {
  int size;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIterations;
  int row_start;
  int row_end;
  int column_start;
  int column_end;
  int *count;
  int *membership;
  struct ppm_pixel *pixel;
  pthread_t tid;
};

// Global Variables
pthread_mutex_t mutex;
pthread_barrier_t mybarrier;
int max_count = 0;

/**
 * Function to compute a section of the Buddhabrot set for a specific thread
 * @param userdata  Pointer to thread_data with image information
 * @return void
 */
void *start(void *user_data) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  struct thread_data *data = (struct thread_data *)user_data;
  struct ppm_pixel *pixel = malloc((size * size) * sizeof(struct ppm_pixel));

  // Assign current thread ID and print sub-image block info
  data->tid = pthread_self();
  printf("Thread %lu) sub-image block: cols (%d, %d) to rows (%d,%d)\n",
         (unsigned long)data->tid, data->column_start, data->column_end,
         data->row_start, data->row_end);

  for (int i = data->row_start; i < data->row_end; i++) {
    for (int j = data->column_start; j < data->column_end; j++) {
      float xfrac = i / (float)size;
      float yfrac = j / (float)size;
      float x0 = xmin + xfrac * (xmax - xmin);
      float y0 = ymin + yfrac * (ymax - ymin);

      float x = 0;
      float y = 0;
      int iter = 0;

      while (iter < maxIterations && x * x + y * y < 2 * 2) {
        float xtmp = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = xtmp;
        iter++;
      }
      // Determines whether a point escapes
      if (iter < maxIterations) {
        data->membership[i * size + j] = 0; // sets membership to 0 if False
      } else {
        data->membership[i * size + j] = 1; // sets membership to 1 if True
      }
    }
  }

  for (int i = data->row_start; i < data->row_end; i++) {
    for (int j = data->column_start; j < data->column_end; j++) {
      if (data->membership[i * size + j] == 0) {
        float xfrac = i / (float)size;
        float yfrac = j / (float)size;
        float x0 = xmin + xfrac * (xmax - xmin);
        float y0 = ymin + yfrac * (ymax - ymin);

        float x = 0;
        float y = 0;

        while (x * x + y * y < 2 * 2) {
          float xtmp = x * x - y * y + x0;
          y = 2 * x * y + y0;
          x = xtmp;

          int yrow = round(size * (y - ymin) / (ymax - ymin));
          int xcol = round(size * (x - xmin) / (xmax - xmin));
          if (yrow < 0 || yrow >= size) {
            continue;
          } // out of range
          if (xcol < 0 || xcol >= size) {
            continue;
          } // out of range
          pthread_mutex_lock(&mutex);
          // increments count at (yrow, xcol)
          data->count[yrow * size + xcol]++;
          // updates max count
          if (data->count[yrow * size + xcol] > max_count) {
            max_count = data->count[yrow * size + xcol];
          }
          pthread_mutex_unlock(&mutex);
        }
      }
    }
  }

  pthread_barrier_wait(&mybarrier);

  float gamma = 0.681;
  float factor = 1.0 / gamma;

  // Writing color at image location (row,col)
  for (int i = data->row_start; i < data->row_end; i++) {
    for (int j = data->column_start; j < data->column_end; j++) {
      float value = 0;
      if (data->count[i * size + j] > 0) {
        value = log(data->count[i * size + j]) / log(max_count);
        value = pow(value, factor);
      }
      data->pixel[i * size + j].red = value * 255;
      data->pixel[i * size + j].green = value * 255;
      data->pixel[i * size + j].blue = value * 255;
    }
  }
  // Free allocated memory
  free(pixel);
  pixel = NULL;
  return (void *)NULL;
}

int main(int argc, char *argv[]) {
  struct timeval tstart, tend;
  // Start time measurement
  gettimeofday(&tstart, NULL);

  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
    case 's':
      size = atoi(optarg);
      break;
    case 'l':
      xmin = atof(optarg);
      break;
    case 'r':
      xmax = atof(optarg);
      break;
    case 't':
      ymax = atof(optarg);
      break;
    case 'b':
      ymin = atof(optarg);
      break;
    case '?':
      printf("usage: %s -s <size> -l <xmin> -r <xmax> "
             "-b <ymin> -t <ymax> -p <numProcesses>\n",
             argv[0]);
      break;
    }
  }
  printf("Generating buddhabrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // Allocating and initializing memory
  struct ppm_pixel *pixel = malloc((size * size) * sizeof(struct ppm_pixel));
  int *membership = malloc((size * size) * sizeof(int));
  int *count = malloc((size * size) * sizeof(int));
  for (int i = 0; i < size * size; i++) {
    count[i] = 0;
    membership[i] = 0;
  }

  // random number generator with current time
  srand(time(0));

  pthread_mutex_init(&mutex, NULL);
  pthread_barrier_init(&mybarrier, NULL, numProcesses);

  // compute image
  // Create threads for the creation of the Buddhabrot set
  pthread_t threads[numProcesses];
  struct thread_data data[numProcesses];
  int subsize = size / 2; // assume multiple of 4
  pthread_t thread_id = pthread_self();
  // Start thread creation and computation
  for (int i = 0; i < numProcesses; i++) {
    int row_block = i / 2;
    int col_block = i % 2;
    data[i].size = size;
    data[i].xmin = xmin;
    data[i].xmax = xmax;
    data[i].ymin = ymin;
    data[i].ymax = ymax;
    data[i].maxIterations = maxIterations;
    data[i].pixel = pixel;
    data[i].count = count;
    data[i].membership = membership;
    data[i].row_start = row_block * subsize;
    data[i].row_end = (row_block + 1) * subsize;
    data[i].column_start = col_block * subsize;
    data[i].column_end = (col_block + 1) * subsize;
    // Create a thread to compute a block of the Buddhabrot set
    pthread_create(&threads[i], NULL, start, (void *)&data[i]);
  }
  // Waits for all threads to finish
  for (int i = 0; i < numProcesses; i++) {
    pthread_join(threads[i], NULL);
  }
  // Print thread completion status
  for (int i = 0; i < numProcesses; i++) {
    printf("Thread %lu) finished\n", (unsigned long)data[i].tid);
  }

  // Measure the total time taken for computation
  gettimeofday(&tend, NULL);
  float timer =
      tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec) / 1.e6;
  printf("Computed buddhabrot set (%dx%d) in %f seconds\n", size, size, timer);

  // Writes the image to a PPM file
  char filename[100];
  sprintf(filename, "buddhabrot-%d-%ld.ppm", size, time(0));
  printf("Writing file: %s\n", filename);
  write_ppm(filename, (struct ppm_pixel *)pixel, size, size);

  // Free allocated memory
  free(pixel);
  pixel = NULL;
  free(membership);
  membership = NULL;
  free(count);
  count = NULL;
  pthread_mutex_destroy(&mutex);
  pthread_barrier_destroy(&mybarrier);
}
