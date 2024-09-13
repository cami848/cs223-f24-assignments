/***************************************************
 * mathgame.c
 * Author: Camille Lopez
 * Date: 09/06/2024
 * Implements a math game that allows user interaction
 */

#include <stdio.h>
#include <stdlib.h>

int main() {

  int rounds, userAnswer, correctAnswers = 0;

  printf("Welcome to Math Game!\n");
  printf("How many rounds do you want to play? ");
  scanf("%d", &rounds);

  // Iterates through each round of the game
  for (int i = 0; i < rounds; i++) {
    int num1 = rand() % 9 + 1;
    int num2 = rand() % 9 + 1;
    int solution = num1 + num2;
    printf("%d + %d = ? ", num1, num2);
    scanf("%d", &userAnswer);

    // Checks if the user's answer is correct
    if (userAnswer == solution) {
      printf("Correct!\n");
      correctAnswers++; // Adds 1 to the count of correct answers
    } 
    else {
      printf("Incorrect :(\n");
    }
  }

  printf("You answered %d/%d correctly.\n", correctAnswers, rounds);
  return 0;
}
