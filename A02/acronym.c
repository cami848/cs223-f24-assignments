/***************************************************
 * acronym.c
 * Author: Camille Lopez
 * Date: 09/10/2024
 * Implements an interactive program that takes a phrase
 * and creates an acronym
 */

#include <stdio.h>
#include <string.h>

int main() {
  char phrase[1024];
  char newPhrase[1024] = {0};
  int j = 0;

  printf("Enter a phrase: ");
  scanf("%[^\n]%*c", phrase);

  for (int i = 0; i < strlen(phrase); i++) {
    char letter = phrase[i];
    // Checks if the letter is capitlizied
    if (65 < letter && letter < 90) {
      newPhrase[j] = letter; // Stores captilized letter to create acronym
      j++;
    }
    newPhrase[j] = '\0';
  }
  printf("Your acronym is %s\n", newPhrase);
  return 0;
}
