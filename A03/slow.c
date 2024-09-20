/*----------------------------------------------
 * Author: Camille Lopez
 * Date: 9/20/2024
 * Description: This program turns the user's
 * inputted phrase into ent-speech based on
 * thier inputted pause length.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  int *length;
  char *buff;
  char *letter;

  length = malloc(sizeof(int));
  buff = malloc(sizeof(char) * 33);
  letter = malloc(sizeof(char) * 33);

  printf("Pause length: ");
  scanf("%d", length);
  printf("Text: ");
  scanf("%s", buff);

  for (int i = 0; i < strlen(buff); i++) {
    *letter = buff[i];
    printf("%c", *letter);
    for (int i = 0; i < *length; i++) {
      printf(".");
    }
  }
  printf("\n");

  free(length);
  free(buff);
  free(letter);
  return 0;
}
