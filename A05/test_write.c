/*----------------------------------------------
 * Author: Camille Lopez
 * Date: 10/4/2024
 * Description: Tests write_ppm function and
 * prints its contents
 ---------------------------------------------*/

#include "read_ppm.h"
#include "write_ppm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  int w, h;

  // Reads the pixel data from PPM file
  struct ppm_pixel *pixels = read_ppm("feep-raw.ppm", &w, &h);

  // Writes the data from pixels into "test.ppm"
  write_ppm("test.ppm", pixels, w, h);
  free(pixels);

  // Reads the data from "test.ppm" into pixels
  pixels = read_ppm("test.ppm", &w, &h);

  // Prints header with filename and dimensions
  printf("Testing file %s: %d %d\n", "test.ppm", w, h);

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
