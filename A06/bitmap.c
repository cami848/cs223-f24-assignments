/*----------------------------------------------
 * Author: Camille Lopez
 * Date: 10/8/2024
 * Description: This program reads in a single
 *  64-bit unsigned integer (e.g. unsigned long)
 * and outputs it as an 8x8 1bpp sprite.
 ---------------------------------------------*/
#include <stdio.h>

int main() {
  unsigned long img;
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n", img);
  for (int i = 1; i <= 64; i++) {
    // A 64-bit mask that obtains the left-most bit
    unsigned long leftMask = 0x1ul << (63 - i + 1);
    unsigned long left = (leftMask & img);
    if (left) {
      printf("1");
    } else {
      (printf("0"));
    }
    //Starts a new line after every 8 bits
    if (i % 8 == 0) {
      printf("\n");
    }
  }
  return 0;
}
