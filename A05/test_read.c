/*----------------------------------------------
 * Author: Camille Lopez
 * Date: 10/4/2024
 * Description: Tests read_ppm function and
 * prints its contents
 ---------------------------------------------*/
#include "read_ppm.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  int w, h;
  char *filename = "feep-raw.ppm";

  // Reads pixel data from the PPM file
  struct ppm_pixel *pixels = read_ppm("feep-raw.ppm", &w, &h);

  // Prints header with filename and dimensions
  printf("Testing file %s: %d %d\n", filename, w, h);

  // Prints out the grid of pixel RGB values
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      printf("(%d,%d,%d) ", pixels[i * h + j].red, pixels[i * h + j].green,
             pixels[i * h + j].blue);
    }
    printf("\n");
  }

  free(pixels);
  pixels = NULL;
  return 0;
}
