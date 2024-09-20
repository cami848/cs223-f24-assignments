/*----------------------------------------------
 * Author: Camille Lopez
 * Date: 09/20/2024
 * Description: This program reads data from a
 * csv and places it into a struct song array in
 * order to prints the songs in a formatted fashion
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/**
 * Iterates over an array of song structs and prints each song's attributes
 * to the console in a formatted fashion
 * @param song A pointer to an array of song structs
 * @param size The number of elements of the array
 * @return void
 */
void printSongs(struct song *song, int size) {
  for (int i = 0; i < size; i++) {
    printf("%-2d) %-25s    artist: %-25s   duration: %d:%02d   danceability: "
           "%.3f   energy: %.3f   tempo: %.3f   valence %.3f\n",
           i, song[i].title, song[i].artist, song[i].durationMin,
           song[i].durationSec, song[i].danceability, song[i].energy,
           song[i].tempo, song[i].valence);
  }
}

/**
 * Reads song data from a CSV and populates an array of
 * struct song with the song attributes
 * @param size A pointer to an int of the size song
 * @return struct
 */
struct song *readSongs(int *size) {
  FILE *infile;
  char buffer[100];

  infile = fopen("songlist.csv", "r");
  if (infile == NULL) {
    printf("Error: unable to open file %s\n", "songlist.csv");
    return NULL;
  }

  // Gets the number of songs in the csv
  fgets(buffer, sizeof(buffer), infile);
  *size = atoi(strtok(buffer, ","));
  // Skips the second line of the csv
  fgets(buffer, sizeof(buffer), infile);

  struct song *songList = malloc(*size * sizeof(struct song));

  // Iterates through the csv and parses the lines into tokens
  for (int i = 0; i < *size; i++) {
    // Retrieves song title
    fgets(buffer, sizeof(buffer), infile);
    char *token = strtok(buffer, ",");
    strcpy(songList[i].title, token);
    // Retrieves artist name
    token = strtok(NULL, ",");
    strcpy(songList[i].artist, token);
    // Retrieves song duration
    token = strtok(NULL, ",");
    int durationMS = atoi(token) / 1000;
    songList[i].durationMin = durationMS / 60;
    songList[i].durationSec = durationMS % 60;
    // Retrieves song danceability
    token = strtok(NULL, ",");
    songList[i].danceability = atof(token);
    // Retrieves song energy
    token = strtok(NULL, ",");
    songList[i].energy = atof(token);
    // Retrieves song tempo
    token = strtok(NULL, ",");
    songList[i].tempo = atof(token);
    // Retrieves song valence
    token = strtok(NULL, ",");
    songList[i].valence = atof(token);
  }
  fclose(infile);
  return songList;
}

int main() {
  printf("Welcome to Camille's Struct's Song List.\n\n");
  int size = 0;
  struct song *songList = readSongs(&size);
  printSongs(songList, size);
  free(songList);
  return 0;
}
