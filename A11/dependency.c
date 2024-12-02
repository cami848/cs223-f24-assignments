/*----------------------------------------------
 * Author: Camille Lopez
 * Date:12/1/2024
 * Description: This program implements a
 *  implement a program that uses N threads to
 *  build a binary search tree of a given set
 * of files.
 ---------------------------------------------*/
#include "tree.h"
#include <pthread.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

struct thread_data {
  char **argv;
  int start_index;
  int end_index;
  int files;
  pthread_t tid;
  struct tree_node **root;
};

// Global Variables
pthread_mutex_t mutex;
pthread_barrier_t mybarrier;
int size = 200;
int count = 0;

/**
 * Function that inserts file data into a binary search tree
 * @param user_data  Pointer to thread_data
 * @return void
 */
void *binary_tree(void *user_data) {
  struct thread_data *data = (struct thread_data *)user_data;

  printf("Thread %ld processing %d files (arguments %d to %d)\n", data->tid,
         data->end_index - data->start_index, data->start_index,
         data->end_index - 1);

  pthread_barrier_wait(&mybarrier);

  for (int i = data->start_index; i < data->end_index; i++) {
    pthread_mutex_lock(&mutex);
    // Insert file into the binary tree
    *(data->root) = insert(data->argv[i], *(data->root));
    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  struct timeval tstart, tend;
  // Start time measurement
  gettimeofday(&tstart, NULL);
  // Check to make sure the input form is correct
  if (argc != 3) {
    printf("Usage: ./dependency <number of threads> <list of "
           "files>\n");
    pthread_exit(NULL);
  } else {
    char *command = argv[2]; // Saves the find command

    int N = atoi(argv[1]);
    int totalFiles = 0;
    char buffer[100];

    char **files = (char **)malloc(100 * sizeof(char *));
    ;
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
      perror("Error running find command");
      return 1;
    }
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
      buffer[strcspn(buffer, "\n")] = '\0'; // Remove the newline character
      files[totalFiles] = strdup(buffer); // Allocate memory for each file path
      totalFiles++;
    }
    pclose(fp);

    int *line_count = malloc(N * sizeof(int));
    ;
    char *keyword = "#include";
    int extra_files = totalFiles % N;
    int numFiles = totalFiles / N;

    // If there are no files, exit
    if (totalFiles == 0) {
      printf("No files found matching the pattern.\n");
      return 0;
    }

    // random number generator with current time
    srand(time(0));
    pthread_mutex_init(&mutex, NULL);
    pthread_barrier_init(&mybarrier, NULL, N);
    printf("Processing %d files\n", totalFiles);
    pthread_t *threads = malloc(N * sizeof(pthread_t));
    struct thread_data *data = malloc(N * sizeof(struct thread_data));
    pthread_mutex_init(&mutex, NULL);
    struct tree_node *root = NULL;
    // Start thread creation
    for (int i = 0; i < N; i++) {
      data[i].start_index = i * numFiles + (i < extra_files ? i : extra_files);
      // Each thread will handle numFiles, plus one extra file if needed
      data[i].end_index =
          data[i].start_index + numFiles + (i < extra_files ? 1 : 0);
      data[i].files = numFiles;
      data[i].argv = files;
      data[i].tid = i;
      data[i].root = &root;
      // Create a thread to search through N files
      pthread_create(&threads[i], NULL, binary_tree, (void *)&data[i]);
    }
    // Waits for all threads to finish
    for (int i = 0; i < N; i++) {
      pthread_join(threads[i], NULL); // Wait for each thread to finish
    }
    gettimeofday(&tend, NULL);
    float timer =
        tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec) / 1.e6;
    printf("Elapsed time is %f\n", timer);
    while (1) {
      char input[256];
      printf("$ ");
      scanf("%s", input);
      // Check if the first argument is "quit" to terminate the program
      if (strcmp(input, "quit") == 0) {
        // Free allocated memory and cleanup before exiting
        for (int i = 0; i < totalFiles; i++) {
          free(files[i]);
        }
        free(files);
        free(threads);
        free(data);
        clear(root);
        free(line_count);
        pthread_mutex_destroy(&mutex);
        pthread_barrier_destroy(&mybarrier);
        exit(0);

      } else if (strcmp(input, "list") == 0) {
        // Call the tree's in-order traversal to list all filenames
        pthread_mutex_lock(&mutex);
        printSorted(root); // Print sorted filenames
        pthread_mutex_unlock(&mutex);
      } else {
        // Search for the node (file) in the binary tree
        struct tree_node *node = find(input, root); // Search the tree
        if (node) {
          printf("%s has the following dependencies:\n", node->data.name);
          // Open the file and search for dependencies
          FILE *infile;
          infile = fopen(node->data.name, "r");

          while (fgets(buffer, sizeof(buffer), infile) != NULL) {
            // Check if the line contains the keyword "#include"
            char *line = strstr(buffer, keyword);
            if (line != NULL) {
              char dependency[100];
              // Extract the dependency between quotes or angle brackets
              char *start = strchr(buffer, '"');
              if (start) {
                char *end = strchr(start + 1, '"');
                int word_length = end - start - 1;
                strncpy(dependency, start + 1, word_length);
                dependency[word_length] = '\0';
                printf("%s\n", dependency);

              } else {
                char *start = strchr(buffer, '<');
                char *end = strchr(start + 1, '>');
                int word_length = end - start - 1;
                strncpy(dependency, start + 1, word_length);
                dependency[word_length] = '\0';
                printf("%s\n", dependency);
              }
            }
          }

          fclose(infile);
        } else {
          printf("%s not found\n", input);
        }
      }
    }

    // Clear the binary tree
    clear(root);

    // Destroy the mutex and barrier
    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&mybarrier);

    // Free allocated memory
    for (int i = 0; i < totalFiles; i++) {
      free(files[i]);
    }
    free(files);
    free(threads);
    free(line_count);
    free(data);
    return 0;
  }
}