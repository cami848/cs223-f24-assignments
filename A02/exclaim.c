/***************************************************
 * exclaim.c
 * Author: Camille Lopez
 * Date: 09/11/2024
 * Implements an interactive program that takes a word
 * and replaces the lowercase letters with a random
 * symbol picked from a set of 4 symbols.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char getSymbol() {
  char symbols[] = {'@', '!', '#', '*'};
  int index = rand() % 4;
  return symbols[index];
}

int main() {

  char buff[32];

  printf("Enter a word: ");
  scanf("%[^\n]%*c", buff);

  for (int i = 0; i < strlen(buff); i++) {
    // Checks if the character is lowercase
    if (96 < buff[i] && buff[i] < 122) {
      buff[i] = getSymbol(); // Replaces lowercase letter with symbol
    } 
    else {
      continue;
    }
  }
  printf("OMG! %s\n", buff);
  return 0;
}
