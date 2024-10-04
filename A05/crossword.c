/*----------------------------------------------
 * Author: Camille Lopez
 * Date: 10/3/2024
 * Description: This program takes two inputted
 * words from the user, checks if they share a 
 * common letter, and if so, creates a crossword
 * where the words intersect based on their
 * shared common letter.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Prints the 2D Array in a crossword format using the two inputted words
 * @param two_d_array the array will store the grid
 * @param row the number of rows
 * @param column the number of columns
 * @param index1 points to the row where the common letter is stored
 * @param index2 points to the column where the common letter is stored
 * @param word1 points to the first inputted word
 * @param word2 points to the second inputted word
 * @return void
 */
void print_crossword(char **two_d_array, int row, int column, int *index1,
                     int *index2, char *word1, char *word2) {
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < column; j++) {
      if (i == *index1) {
        two_d_array[i][j] = word2[j];
      } 
      else if (j == *index2) {
        two_d_array[i][j] = word1[i];
      }
      else {
        two_d_array[i][j] = '.';
      }
      printf(" %s ", &two_d_array[i][j]);
    }
    printf("\n");
  }
};

/**
 * Finds the shared letter between the first and second word
 * @param word1 points to the first inputted word
 * @param word2 points to the second inputted word
 * @param common points to the shared character between the two words
 * @param index1 points to the row where the common letter is stored
 * @param index2 points to the column where the common letter is stored
 * @return void
 */
void find_common_letter(char *word1, char *word2, char *common, int *index1,
                        int *index2) {
  *common = '\0';
  for (int i = 0; i < strlen(word1); i++) {
    for (int j = 0; j < strlen(word2); j++) {
      if (word1[i] == word2[j]) {
        *index1 = i;
        *index2 = j;
        *common = word1[i];
        return;
      }
    }
  }
};

int main(int argc, char *argv[]) {
  // Check to make sure the input form is correct
  if (argc < 3 || argc > 3) {
    printf("Usage: %s <word1> <word2>\n", argv[0]);
    exit(0);
  } 
  else {
    int row, column, index1, index2;
    char common, *word1, *word2;
    // Stores the inputted args into word1 and word2
    word1 = argv[1];
    word2 = argv[2];
    // Stores the length of the words into row and column
    row = strlen(word1);
    column = strlen(word2);
    // Allocates a 2D array to store the grid
    char **two_d_array = malloc(sizeof(char *) * (row));
    for (int i = 0; i < row; i++) {
      two_d_array[i] = malloc(sizeof(char) * (column));
    }

    find_common_letter(word1, word2, &common, &index1, &index2);
    if (common == '\0') {
      printf("No common letter found!\n");
      exit(0);
    }
    print_crossword(two_d_array, row, column, &index1, &index2, word1, word2);

    // Frees allocated memory
    for (int i = 0; i < row; i++) {
      free(two_d_array[i]);
    };
    free(two_d_array);
    two_d_array= NULL;
  }
  return 0;
}