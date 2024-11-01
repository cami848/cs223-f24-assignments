/*----------------------------------------------
 * Author: Camille Lopez
 * Date:10/28/2024
 * Description: This program  spawns processes
 * according to a provided diagram.
 ---------------------------------------------*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * Spawns processes using fork() to print messages
 * @return void
 */
void forky() {
  __pid_t pid;
  printf("%d: A0\n", getpid());
  pid = fork();

  if (pid != 0) {
    printf("%d: B0\n", getpid());
    pid = fork();
    if (pid != 0) {
      printf("%d: C0\n", getpid());
    } else {
      printf("%d: C1\n", getpid());
    }
  } else {
    printf("%d: B1\n", getpid());
  }
  printf("%d: Bye\n", getpid());
}

int main() {
  forky();
  return 0;
}
