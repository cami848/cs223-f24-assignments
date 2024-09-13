/***************************************************
 * songs.c
 * Author: Camille Lopez
 * Date: 09/12/2024
 * Implements an interactive program that gives 3 songs
 * with four of its attributes and allows the user to
 *  change any attribute of any song of their choosing.
 */

#include <stdio.h>
#include <string.h>

struct song {
  char artist[32];    // String representing the name of the artist
  char title[32];     // String representing the title of the song
  int durationMin;    // Duration of the song in minutes
  int durationSec;    // Duration of the song in seconds
  float danceability; // Danceability score of the song
};

/**
 * Iterates over an array of song structs and prints each song's attributes
 * to the console in a formatted fashion
 * @param song A pointer to an array of song structs
 * @param size The number of elements of the array
 * @return void
 */
void songList(struct song *song, int size) {
  for (int i = 0; i < size; i++) {
    printf("%d) %-20s    artist: %-20s   duration: %d:%02d     danceability: "
           "%f \n",
           i, song[i].title, song[i].artist, song[i].durationMin,
           song[i].durationSec, song[i].danceability);
  }
}

/**
 * Updates a specfic attribute of a struct song based on user input
 * @param song A pointer to a song struct
 * @param attribute A string specifying the attribute to update
 * @return void
 */
void updateAttribute(struct song *song, const char *attribute) {
  char newTitle[32];
  if (strcmp(attribute, "title") == 0) {
    printf("Enter a title: ");
    scanf(" %[^\n]", newTitle);
    strcpy(song->title, newTitle);
  } 
  else if (strcmp(attribute, "artist") == 0) {
    char newArtist[32];
    printf("Enter an artist: ");
    scanf(" %[^\n]", newArtist);
    strcpy(song->artist, newArtist);

  } 
  else if (strcmp(attribute, "duration") == 0) {
    int newMin;
    int newSec;
    printf("Enter a duration (minutes): ");
    scanf(" %d", &newMin);
    song->durationMin = newMin;

    printf("Enter a duration (seconds): ");
    scanf(" %d", &newSec);
    song->durationSec = newSec;
  } 
  else if (strcmp(attribute, "danceability") == 0) {
    float newDanceability;

    printf("Enter danceability: ");
    scanf(" %f", &newDanceability);
    song->danceability = newDanceability;
  } 
  else {
    printf("Unknown attribute!\n");
  }
}

int main() {
  printf("Welcome to Steven Struct's Song List.\n\n");
  struct song song1, song2, song3;
  struct song songs[3];
  int id = 0;
  char attribute[32];

  // song1 access field value
  strcpy(song1.artist, "Blood Orange");
  strcpy(song1.title, "Champagne Coast");
  song1.durationMin = 4;
  song1.durationSec = 50;
  song1.danceability = 0.35;

  // song2 access field values
  strcpy(song2.artist, "Charli xcx");
  strcpy(song2.title, "B2b");
  song2.durationMin = 2;
  song2.durationSec = 58;
  song2.danceability = 1.00;

  // song3 access field values
  strcpy(song3.artist, "Frank Ocean");
  strcpy(song3.title, "Novacane");
  song3.durationMin = 5;
  song3.durationSec = 2;
  song3.danceability = 0.20;

  songs[0] = song1;
  songs[1] = song2;
  songs[2] = song3;
  songList(songs, 3);

  int length = 30;
  for (int i = 0; i < length; i++) {
    printf("=");
  }
  printf("\n");
  printf("Enter a song id to edit [0, 1, 2]: ");
  scanf("%d", &id);

  if (id >= 0 && id <= 2) {
    printf("Which attribute do you wish to edit? [artist, title, duration, "
           "danceability]: ");
    scanf("%s", attribute);
    updateAttribute(&songs[id], attribute);
    songList(songs, 3);
  } 
  else {
    printf("Invalid choice!\n");
  }
  return 0;
}