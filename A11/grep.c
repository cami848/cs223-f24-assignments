/*----------------------------------------------
 * Author: Camille Lopez
 * Date:12/1/2024
 * Description: This program implements a
 * simplified version of the bash command grep
 * that uses N threads to search for a keyword
 * in a set of files.
 ---------------------------------------------*/

#include <assert.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

struct thread_data {
  char **argv;
  int start_index;
  int end_index;
  int *line_count;
  char **files;
  char *keyword;
  pthread_t tid;
};

// Global Variables
pthread_mutex_t mutex;
pthread_barrier_t mybarrier;

/**
 * Function that searchs for a keyword in a set of files
 * @param user_data  Pointer to thread_data
 * @return void
 */
void *grep(void *user_data) {
  struct thread_data *data = (struct thread_data *)user_data;
  int count = 0;
  FILE *infile;
  char buffer[100];

  printf("Thread %ld searching %d files (%d to %d)\n", data->tid,
         data->end_index - data->start_index, data->start_index,
         data->end_index - 1);

  pthread_barrier_wait(&mybarrier);

  for (int i = data->start_index; i < data->end_index; i++) {
    infile = fopen(data->files[i], "r");
    if (infile == NULL) {
      pthread_mutex_lock(&mutex);
      fprintf(stderr, "Thread %ld: Error opening file: %s \n", data->tid,
              data->files[i]);
      pthread_mutex_unlock(&mutex);
      exit(0);
    }
    // Read each line of the file
    while (fgets(buffer, sizeof(buffer), infile) != NULL) {
      // Check if the line contains the keyword
      if (strstr(buffer, data->keyword)) {
        pthread_mutex_lock(&mutex);
        printf("%ld) %s:%s", data->tid, data->files[i], buffer);
        pthread_mutex_unlock(&mutex);
        count++;
      }
    }
    fclose(infile);
  }
  // Store the count of matching lines found by this thread
  data->line_count[data->tid] = count;
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  struct timeval tstart, tend;
  // Start time measurement
  gettimeofday(&tstart, NULL);
  // Check to make sure the input form is correct
  if (argc < 4 || argc > 4) {
    printf("Usage: ./grep <number of threads> <keyword> <list of files>\n");
    pthread_exit(NULL);
  } else {
    char *command = argv[3]; // The find command (e.g., 'find code -name "*.h"')

    int N = atoi(argv[1]);
    int totalFiles = 0;
    char buffer[100];

    char **files = malloc(100 * sizeof(char *));
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
    char *keyword = argv[2];
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
    printf("Searching %d files for keyword: %s\n", totalFiles, keyword);
    pthread_t *threads = malloc(N * sizeof(pthread_t));
    struct thread_data *data = malloc(N * sizeof(struct thread_data));
    // Start thread creation
    for (int i = 0; i < N; i++) {
      data[i].start_index = i * numFiles + (i < extra_files ? i : extra_files);
      // Each thread will handle numFiles, plus one extra file if needed
      data[i].end_index =
          data[i].start_index + numFiles + (i < extra_files ? 1 : 0);
      data[i].line_count = line_count;
      data[i].files = files;
      data[i].keyword = keyword;
      data[i].tid = i;
      // Create a thread to search through N files
      pthread_create(&threads[i], NULL, grep, (void *)&data[i]);
    }
    // Waits for all threads to finish
    for (int i = 0; i < N; i++) {
      pthread_join(threads[i], NULL);
    }
    gettimeofday(&tend, NULL);
    float timer =
        tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec) / 1.e6;
    printf("Elapsed time is %f\n", timer);
    int total_lines = 0;
    for (int i = 0; i < N; i++) {
      printf("Thread %d found %d lines containing keyword: %s\n", i,
             line_count[i], keyword);
      total_lines += line_count[i];
    }
    // Free allocated memory
    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&mybarrier);
    for (int i = 0; i < totalFiles; i++) {
      free(files[i]);
    }
    free(files);
    free(threads);
    free(data);
    free(line_count);
  }
  return 0;
}
