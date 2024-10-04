/*----------------------------------------------
 * Author: Camille Lopez
 * Date: 10/4/2024
 * Description Reads PPM files stored in binary 
 * format and returns the data in a 2D array
 ---------------------------------------------*/
#include "read_ppm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Choose *one* to implement (do not remove the other one!)

/**
 * Reads binary PPM files
 * @param filename points to the name of the file
 * @param w the width of the image
 * @param h the height of the image
 * @return struct ppm_pixel
 */
struct ppm_pixel *read_ppm(const char *filename, int *w, int *h) {
  FILE *infile;
  char buffer[100];
  int max;

  infile = fopen(filename, "r");
  // Checks if inputted file is found
  if (infile == NULL) {
    printf("Error: unable to open file %s\n", filename);
    exit(1);
  }

  // Skips the first two lines of the file
  fgets(buffer, sizeof(buffer), infile);
  fgets(buffer, sizeof(buffer), infile);

  // Reads image dimensions and max color value
  fscanf(infile, "%d %d\n%d\n", w, h, &max);

  struct ppm_pixel *pixel = malloc((*w * *h) * sizeof(struct ppm_pixel));

  // reads pixel data from filr into *pixel
  fgets((char *)pixel, (*w * *h) * sizeof(struct ppm_pixel), infile);

  fclose(infile);
  return pixel;
}

struct ppm_pixel **read_ppm_2d(const char *filename, int *w, int *h) {
  return NULL;
}
