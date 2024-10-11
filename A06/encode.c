/*----------------------------------------------
 * Author: Camille Lopez
 * Date:10/11/2024
 * Description: This program reads in a PPM
 * file (raw, or binary, format) and asks the
 *  user for a message to embed within it.
 ---------------------------------------------*/
#include "read_ppm.h"
#include "write_ppm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void changeFilename(char *input, char *output) {
  char *period = strrchr(input, '.'); // Finds the file extension

  if (period == NULL) {
    // Appends "-encoded" to filename
    strcpy(output, input);
    strcat(output, "-encoded");
  } else {
    // Copies everything before the extension
    strncpy(output, input, period - input);
    // Null-terminates the base name
    output[period - input] = '\0';

    // Adds "-encoded" to the original filename
    strcat(output, "-encoded");
    strcat(output, period);
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("usage: encode <file.ppm>\n");
    return 0;
  }
  int w, h, max;
  char *filename = argv[1];

  // Reads pixel data from the PPM file
  struct ppm_pixel *pixels = read_ppm(filename, &w, &h);

  unsigned char *colors = (unsigned char *)pixels;
  max = (w * h * 3) / 8;
  char phrase[100];
  char output[256];

  // Prints header with filename and dimensions
  printf("Reading %s: with width %d and height %d\n", filename, w, h);
  printf("Max number of characters in the image: %d\n", max - 1);
  printf("Enter a phrase: ");
  scanf(" %[^\n]%*c", phrase);

  unsigned char rightMask;
  unsigned char newBit;
  unsigned char newColor;

  // iterates through the number of bytes in the array colors
  for (int i = 0; i < max * 8; i++) {
    // stores the least significant bit of colors[i]
    rightMask = colors[i] & 0b1;
    // checks if the current bit position is beyond the length of
    // the phrase in bits
    if (i > strlen(phrase) * 8) {
      newBit = 0;
    } else {
      newBit = (phrase[i / 8] >> (7 - i % 8)) & 0b1;
    }
    // clears the least significant bit of colors[i]
    //  and sets it to newBit
    newColor = (colors[i] ^ rightMask) | newBit;
    // modified value is stored back into colors[i]
    colors[i] = newColor;
  }

  changeFilename(filename, output);

  printf("Writing file %s\n", output);

  // write the new colors data to a new PPM file
  write_ppm(output, (struct ppm_pixel *)colors, w, h);

  // Frees allocated memory
  free(pixels);
  pixels = NULL;
  return 0;
}
