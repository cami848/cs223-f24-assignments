/*----------------------------------------------
 * Author: Camille Lopez
 * Date:10/31/2024
 * Description: This program implements a
 * simple shell.
 ---------------------------------------------*/

#include <pwd.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_LIGHT_BLUE "\x1b[94m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_WHITE "\x1b[97m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_LIGHT_GREEN "\x1b[92m"
#define ANSI_COLOR_BROWN "\x1b[33m"
#define ANSI_COLOR_RESET "\x1b[0m"

int main() {
  char hostname[100];
  char cwd[200];
  struct passwd *pw = getpwuid(geteuid());
  gethostname(hostname, sizeof(hostname) - 1);
  getcwd(cwd, sizeof(cwd) - 1);
  hostname[sizeof(hostname) - 1] = '\0';
  cwd[sizeof(cwd) - 1] = '\0';
  using_history();
  stifle_history(500);
  char *args[100];

  printf(
      ANSI_COLOR_WHITE
      "*" ANSI_COLOR_LIGHT_BLUE " " ANSI_COLOR_WHITE "C" ANSI_COLOR_LIGHT_BLUE
      "a" ANSI_COLOR_WHITE "m" ANSI_COLOR_LIGHT_BLUE "i" ANSI_COLOR_WHITE
      "l" ANSI_COLOR_LIGHT_BLUE "l" ANSI_COLOR_WHITE "e" ANSI_COLOR_LIGHT_BLUE
      "'" ANSI_COLOR_WHITE "s" ANSI_COLOR_LIGHT_BLUE " " ANSI_COLOR_WHITE
      "S" ANSI_COLOR_LIGHT_BLUE "h" ANSI_COLOR_WHITE "e" ANSI_COLOR_LIGHT_BLUE
      "l" ANSI_COLOR_WHITE "l" ANSI_COLOR_LIGHT_BLUE " " ANSI_COLOR_WHITE
      "*" ANSI_COLOR_RESET "\n");
  printf(ANSI_COLOR_WHITE " ----------------- " ANSI_COLOR_RESET "\n");

  while (1) {
    // prints a prompt that displays the machine name,
    // the current working directory, and current user
    printf(ANSI_COLOR_WHITE "%s@" ANSI_COLOR_CYAN "%s:" ANSI_COLOR_WHITE
                            "%s" ANSI_COLOR_CYAN " <3" ANSI_COLOR_RESET,
           pw->pw_name, hostname, cwd);
    char *input = readline("$"); // gets user input
    if (input == NULL ||
        strlen(input) == 0) { // Check if input is NULL or empty
      free(input);
      continue;
    }
    add_history(input); // saves user history
    // Tokenize the input string by spaces and store each token in args array
    char *token = strtok(input, " ");
    int i = 0;
    // Loop through the rest of the input, splitting by spaces
    while (token != NULL) {
      args[i] = token;
      token = strtok(NULL, " ");
      i++;
    }
    // Check if the first argument is "exit" to terminate the program
    args[i] = NULL;
    if (strcmp(args[0], "exit") == 0) {
      exit(0);
    }
    pid_t pid;
    pid = fork();   // create new process
    if (pid == 0) { /* child */
      if (execvp(args[0], args) < 0) {
        printf("%s: Command not found.\n", args[0]);
        exit(1);
      }

    } else { /*parent*/
      int status;
      // wait for child to exit
      waitpid(pid, &status, 0);
    }
    free(input);
  }
}