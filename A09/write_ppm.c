#include "write_ppm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void write_ppm(const char *filename, struct ppm_pixel *pixels, int w, int h) {
  FILE *outfile;

  outfile = fopen(filename, "w");
  // Checks if inputted file is found
  if (outfile == NULL) {
    printf("Error: unable to open file %s\n", filename);
    exit(1);
  }
  // Writing PPM header
  fprintf(outfile, "P6\n# Created by Camille\n%d %d \n255\n", w, h);
  // Loops through the file and stores pixel color data into array
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      unsigned char pixel[3] = {pixels[i * h + j].red, pixels[i * h + j].green,
                                pixels[i * h + j].blue};
      // Writes the RGB values into the file
      fwrite(pixel, sizeof(unsigned char), 3, outfile);
    }
  }
  fclose(outfile);
}

void write_ppm_2d(const char *filename, struct ppm_pixel **pxs, int w, int h) {}
