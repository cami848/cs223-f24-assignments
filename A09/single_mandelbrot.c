/*----------------------------------------------
 * Author: Camille Lopez
 * Date:11/04/2024
 * Description: This program outputs a PPM image
 *  of the mandelbrot set.
 ---------------------------------------------*/

#include "read_ppm.h"
#include "write_ppm.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  struct timeval tstart, tend;
  // Start time measurement
  gettimeofday(&tstart, NULL);
  // Default parameters for Mandelbrot set
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
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
      printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n",
             argv[0]);
      break;
    }
  }
  // Print chosen parameters for generating the Mandelbrot set
  printf("Generating mandelbrot with size %dx%d\n", size, size);
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
  // Compute the Mandelbrot set for each pixel
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
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
        pixel[j * size + i] = palette[iter]; // Color corresponding to iteration
      } else {
        pixel[j * size + i].red = 0;
        pixel[j * size + i].green = 0;
        pixel[j * size + i].blue = 0;
      }
    }
  }

  // Writes the image to a PPM file
  char filename[100];
  sprintf(filename, "mandelbrot-%d-%ld.ppm", size, time(0));
  write_ppm(filename, pixel, size, size);

  // Measure the total time taken for computation
  gettimeofday(&tend, NULL);
  float timer =
      tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec) / 1.e6;
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, timer);
  printf("Writing file: %s\n", filename);
  // Free allocated memory
  free(pixel);
  pixel = NULL;
  free(palette);
  palette = NULL;
}
