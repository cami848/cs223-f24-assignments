/*----------------------------------------------
 * Author: Camille Lopez
 * Date: 09/27/2024
 * Description: This program creates a linked 
 * list to store song information from a csv, 
 * while also allowing users to access the song
 *  with the highest danceability before removing
 *  it from the list.
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node *node_ptr;

struct song {
  char artist[32];    // String representing the name of the artist
  char title[32];     // String representing the title of the song
  int durationMin;    // Duration of the song in minutes
  int durationSec;    // Duration of the song in seconds
  float danceability; // Danceability score of the song
  float energy;       // Float representing the song's intensity and activity
  float tempo;        // Float representing the song's estimated tempo (BPM)
  float valence;      // Float representing the song's musical positiveness
};

struct node {
  struct song element; // Struct representing a song
  node_ptr next;       // Pointer representing the next node
};

/**
 * Inserts a node into the front of the linked list
 * @param element The struct song element in struct node
 * @param head A pointer to struct node head of linked list
 * @return struct node
 */
struct node *insert_front(struct song element, struct node *head) {
  struct node *n = malloc(sizeof(struct node));
  if (n == NULL) {
    printf("Error: Out of space\n");
    exit(1);
  }
  n->element = element;
  n->next = head;
  head = n;
  return n;
};

/**
 * Removes node from linked list
 * @param prevNode A pointer to struct node
 * @return void
 */
void remove_node(struct node *prevNode) {
  if (prevNode == NULL || prevNode->next == NULL) {
    printf("Error: Node ");
    return;
  }

  struct node *node = prevNode->next;
  prevNode->next = node->next;
  free(node);
};

/**
 *Clears linked list
 * @param head A pointer to struct node head of linked list
 * @return void
 */
void clear_list(struct node *head) {
  struct node *tempNode;
  while (head != NULL) {
    tempNode = head;
    head = head->next;
    free(tempNode);
  }
  return;
}

/**
 * Finds the node with the highest danceability, prints it,
 *  and removes it from the linked list
 * @param head A pointer to struct node head of linked list
 * @return void
 */
void getDanceability(struct node **head) {
  int i = 0;
  struct node *max = *head; // Pointer to the node with the highest danceability
  struct node *tempNode = (*head)->next; // Start checking from the second node
  struct node *prevNode = *head;         // To keep track of the previous node
  struct node *prevMaxNode = NULL;       // To track the previous node of max

  if (*head == NULL) {
    printf("Error: No songs available.\n");
    return;
  }

  while (tempNode != NULL) {
    if (tempNode->element.danceability > max->element.danceability) {
      max = tempNode;         // Update max pointer
      prevMaxNode = prevNode; // Update the previous max node
    }
    prevNode = tempNode;       // Move prevNode to current tempNode
    tempNode = tempNode->next; // Move tempNode to next node
  }

  printf("-------------------------------------------Most "
         "Danceable-------------------------------------\n\n");
  printf("%-2d) %-25s    artist: %-25s   duration: %d:%02d   danceability: "
         "%.3f   energy: %.3f   tempo: %.3f   valence %.3f\n\n",
         1, max->element.title, max->element.artist, max->element.durationMin,
         max->element.durationSec, max->element.danceability,
         max->element.energy, max->element.tempo, max->element.valence);
  int length = 100;
  for (int i = 0; i < length; i++) {
    printf("-");
  }
  printf("\n\n");

  // Remove the max node
  if (max == *head) {
    // If the max node is the head, update the head pointer
    struct node *temp = *head;
    *head = (*head)->next; // Update head to the next node
    free(temp);
  } else {
    remove_node(prevMaxNode); // Remove the max node using prevMaxNode
  }
}

/**
 * Iterates over a list of struct songs and prints each song's attributes
 * to the console in a formatted fashion
 * @param list A pointer to a list of struct nodes
 * @return void
 */
void printSongs(struct node *list) {
  struct node *song = list;
  int i = 0;
  while (song != NULL) {
    printf("%-2d) %-25s    artist: %-25s   duration: %d:%02d   danceability: "
           "%.3f   energy: %.3f   tempo: %.3f   valence %.3f\n",
           i, song->element.title, song->element.artist,
           song->element.durationMin, song->element.durationSec,
           song->element.danceability, song->element.energy,
           song->element.tempo, song->element.valence);
    i++;
    song = song->next;
  }
};

/**
 * Reads song data from a CSV and populates a linked list
 *  of struct song with the song attributes
 * @return struct node
 */
struct node *readSongs() {
  FILE *infile;
  char buffer[100];

  infile = fopen("songlist.csv", "r");
  if (infile == NULL) {
    printf("Error: unable to open file %s\n", "songlist.csv");
    return NULL;
  }

  // Skips the first line of the csv
  fgets(buffer, sizeof(buffer), infile);

  struct node *songList = NULL;

  // Iterates through the csv and puts each song into a node
  while (fgets(buffer, sizeof(buffer), infile) != NULL) {
    struct song songs;
    // Retrieves song title
    char *token = strtok(buffer, ",");
    strcpy(songs.title, token);
    // Retrieves artist name
    token = strtok(NULL, ",");
    strcpy(songs.artist, token);
    // Retrieves song duration
    token = strtok(NULL, ",");
    int durationMS = atoi(token) / 1000;
    songs.durationMin = durationMS / 60;
    songs.durationSec = durationMS % 60;
    // Retrieves song danceability
    token = strtok(NULL, ",");
    songs.danceability = atof(token);
    // Retrieves song energy
    token = strtok(NULL, ",");
    songs.energy = atof(token);
    // Retrieves song tempo
    token = strtok(NULL, ",");
    songs.tempo = atof(token);
    // Retrieves song valence
    token = strtok(NULL, ",");
    songs.valence = atof(token);
    songList = insert_front(songs, songList);
  }
  fclose(infile);
  return songList;
}

int main() {
  printf("Welcome to Camille's Struct's Song List.\n\n");
  char key;
  int size = 0;
  struct node *songList = readSongs();
  while (1) {
    printSongs(songList);
    int length = 100;
    for (int i = 0; i < length; i++) {
      printf("=");
    }
    printf("\n\n");
    printf(
        "Press 'd' to show the most danceable song (any other key to quit): ");
    key = getchar();
    getchar();
    if (key == 'd') {
      if (songList == NULL) {
        printf("Dataset contains 0 songs\n");
      } else {
        getDanceability(&songList);
      }
    } else {
      // Clears list if user quits program
      clear_list(songList);
      songList = NULL;
      return 0;
    }
  }
  return 0;
}
