/*----------------------------------------------
 * Author: Camille Lopez
 * Date: 10/11/2024
 * Description: This program reads in a PPM file
 *  (raw, or binary, format) and then outputs
 * any message that might be stored in the least
 *  significant bits of each color.
 ---------------------------------------------*/
#include "read_ppm.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("usage: decode <file.ppm>\n");
    return 0;
  }
  int w, h, max;
  char *filename = argv[1];

  // Reads pixel data from the PPM file
  struct ppm_pixel *pixels = read_ppm(filename, &w, &h);
  unsigned char *colors = (unsigned char *)pixels;
  max = (w * h * 3) / 8;

  // Prints header with filename and dimensions
  printf("Reading %s: with width %d and height %d\n", filename, w, h);
  printf("Max number of characters in the image: %d\n", max);

  unsigned char rightMask = 0b1; // Mask to extract the least significant bit
  unsigned char temp_Char;
  char *result = malloc(sizeof(char) * max);
  // Iterates through the image data
  for (int i = 0; i < max * 8; i++) {
    // Extract the least significant bit of the current color byte
    unsigned char bits = (rightMask & colors[i]);
    // Shift the temporary character to the left and add the new bit to the
    // right
    temp_Char = temp_Char << 1 | bits;
    // Checks if we've processed 8 bits
    if (i % 8 == 7) {
      result[i / 8] = temp_Char; // Stores char in result array
      if (temp_Char == '\0') {
        break;
      } else {
        temp_Char = 0; // Resets variable for next character
      }
    }
  }
  printf("%s\n", result);
  // Frees the allocated memory
  free(pixels);
  pixels = NULL;
  free(result);
  result = NULL;
  return 0;
}