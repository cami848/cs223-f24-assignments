/*----------------------------------------------
 * Author: Camille Lopez
 * Date:11/04/2024
 * Description: This program outputs a PPM image
 * of the mandelbrot set using threads.
 ---------------------------------------------*/

#include "read_ppm.h"
#include "write_ppm.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#define numProcesses 4

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
  struct ppm_pixel *pixel;
  struct ppm_pixel *colors;
  pthread_t tid;
};

/**
 * Function to compute a section of the Mandelbrot set for a specific thread
 * @param userdata  Pointer to thread_data with image information
 * @return void
 */
void *mandelbrot(void *userdata) {
  struct thread_data *data = (struct thread_data *)userdata;
  int size = data->size;
  float xmin = data->xmin;
  float xmax = data->xmax;
  float ymin = data->ymin;
  float ymax = data->ymax;
  int maxIterations = data->maxIterations;

  // Assign current thread ID and print sub-image block info
  data->tid = pthread_self();
  printf("Thread %lu) sub-image block: cols (%d, %d) to rows (%d,%d)\n",
         (unsigned long)data->tid, data->column_start, data->column_end,
         data->row_start, data->row_end);

  // Loop through the block of pixels assigned to this thread
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

      if (iter < maxIterations) {
        data->pixel[j * size + i] = data->colors[iter];
      } else {
        data->pixel[j * size + i].red = 0;
        data->pixel[j * size + i].green = 0;
        data->pixel[j * size + i].blue = 0;
      }
    }
  }
  return (void *)NULL;
}

int main(int argc, char *argv[]) {
  struct timeval tstart, tend;
  // Start time measurement
  gettimeofday(&tstart, NULL);
  // Set default parameters for Mandelbrot generation
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
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
  // Print chosen parameters for generating the Mandelbrot set
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  struct ppm_pixel *pixel = malloc((size * size) * sizeof(struct ppm_pixel));
  struct ppm_pixel *palette =
      malloc((maxIterations) * sizeof(struct ppm_pixel));

  // random number generator with current time
  srand(time(0));

  // Generate a random color palette for the Mandelbrot set
  for (int i = 0; i < maxIterations; i++) {
    palette[i].red = rand() % 255;
    palette[i].green = rand() % 255;
    palette[i].blue = rand() % 255;
  }

  // Create threads for the creation of the Mandelbrot set
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
    data[i].colors = palette;
    data[i].row_start = row_block * subsize;
    data[i].row_end = (row_block + 1) * subsize;
    data[i].column_start = col_block * subsize;
    data[i].column_end = (col_block + 1) * subsize;
    // Create a thread to compute a block of the Mandelbrot set
    pthread_create(&threads[i], NULL, mandelbrot, (void *)&data[i]);
  }
  // Waits for all threads to finish
  for (int i = 0; i < 4; i++) {
    pthread_join(threads[i], NULL);
  }
  // Print thread completion status
  for (int i = 0; i < numProcesses; i++) {
    printf("Thread %lu) finished\n", (unsigned long)data[i].tid);
  }
  // Writes the image to a PPM file
  char filename[100];
  sprintf(filename, "mandelbrot-%d-%ld.ppm", size, time(0));
  printf("Writing file: %s\n", filename);
  write_ppm(filename, (struct ppm_pixel *)pixel, size, size);

  // Measure the total time taken for computation
  gettimeofday(&tend, NULL);
  float timer =
      tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec) / 1.e6;
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, timer);

  // Free allocated memory
  free(pixel);
  pixel = NULL;
  free(palette);
  palette = NULL;
}
