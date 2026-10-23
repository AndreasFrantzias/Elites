#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

// GLOBAL VARIABLES
char bus_lines[12][60] = {"040 PEIRAIAS - SINTAGMA", "040 SINTAGMA - PEIRAIAS", "10 TZITZIFIES - XALANDRI", "10 XALANDRI - TZITZIFIES", "224 KAISARIANI - EL. VENIZELOY", "224 EL. VENIZELOY - KAISARIANI", "250 PANEPISTIMIOUPOLI - ST. EVAGGELISMOS", "550 P. FALIRO - KIFISIA", "550 KIFISIA - P. FALIRO", "608 GALATSI - AKADIMIA - NEKR. ZOGRAFOY", "608 NEKR. ZOGRAFOY - AKADIMIA - GALATSI"};
int*** departures = NULL;
char*** bus_stops = NULL;
int bus_indices_040[38];
int bus_indices_040_return[38];
int bus_indices_10[47];
int bus_indices_10_return[41];
int bus_indices_224[34];
int bus_indices_224_return[34];
int bus_indices_250[26];
int bus_indices_550[56];
int bus_indices_550_return[56];
int bus_indices_608[41];
int bus_indices_608_return[45];


// IGNORE THESE FUNCTIONS -> IT JUST PARSES THE INFORMATION
// FOUND IN THE .TXT FILES
// -- IGNORE --

int** parse_one_departure(char* filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    fprintf(stderr, "File Error.\n");
    exit(1);
  }
  int** dep = malloc(150 * sizeof(int*));
  if (!dep) {
    fprintf(stderr, "Memory Allocation Error!\n");
    exit(1);
  }
  int line = 1, hour, min;
  dep[0] = malloc(2 * sizeof(int));
  if (!dep[0]) {
    fprintf(stderr, "Memory Allocation Error!\n");
    exit(1);
  }
  dep[0][0] = 1;
  dep[0][1] = 0;
  while (fscanf(fp, "%d:%d", &hour, &min) == 2) {
    dep[line] = malloc(2 * sizeof(int));
    if (!dep[line]) {
      fprintf(stderr, "Memory Allocation Error!\n");
      exit(1);
    }
    dep[line][0] = hour;
    dep[line++][1] = min;
  }
  dep[line] = malloc(2 * sizeof(int));
  if (!dep[line]) {
    fprintf(stderr, "Memory Allocation Error!\n");
    exit(1);
  }
  dep[line][0] = hour;
  dep[line][1] = min;
  dep[0][1] = line;
  fclose(fp);
  return dep;
}

void parse_departures() {
  departures = malloc(11 * sizeof(int**));
  if (!departures) {
    fprintf(stderr, "Memory Allocation Error!\n");
    exit(1);
  }
  departures[0] = parse_one_departure("./departures/dep_040.txt");
  departures[1] = parse_one_departure("./departures/dep_040_return.txt");
  departures[2] = parse_one_departure("./departures/dep_10.txt");
  departures[3] = parse_one_departure("./departures/dep_10_return.txt");
  departures[4] = parse_one_departure("./departures/dep_224.txt");
  departures[5] = parse_one_departure("./departures/dep_224_return.txt");
  departures[6] = parse_one_departure("./departures/dep_250.txt");
  departures[7] = parse_one_departure("./departures/dep_550.txt");
  departures[8] = parse_one_departure("./departures/dep_550_return.txt");
  departures[9] = parse_one_departure("./departures/dep_608.txt");
  departures[10] = parse_one_departure("./departures/dep_608_return.txt");
  return;
}

void free_departures() {
  for (int i = 0; i < 11; i++) {
    int help = departures[i][0][1];
    for (int j = 0; j <= help; j++) {
      free(departures[i][j]);
    }
    free(departures[i]);
  }
  free(departures);
  return;
}

void parse_stops() {
  FILE *fp = fopen("./bus_stops/standalone_stops.txt", "r");
  if (!fp) {
    fprintf(stderr, "File Opening Error!\n");
    exit(1);
  }

  bus_stops = malloc(366 * sizeof(char**));
  if (!bus_stops) {
    fprintf(stderr, "Memory Allocation Error!\n");
    exit(1);
  }
  int i = 0;
  char code[70], name[70];
  while (fscanf(fp, "%s %s", code, name) == 2) {
    bus_stops[i] = malloc(2 * sizeof(char*));
    if (!bus_stops[i]) {
      fprintf(stderr, "Memory Allocation Error!\n");
      exit(1);
    }
    bus_stops[i][0] = malloc(60 * sizeof(char));
    if (!bus_stops[i][0]) {
      fprintf(stderr, "Memory Allocation Error!\n");
      exit(1);
    }
    strcpy(bus_stops[i][0], code);
    bus_stops[i][1] = malloc(60 * sizeof(char));
    if (!bus_stops[i][1]) {
      fprintf(stderr, "Memory Allocation Error!\n");
      exit(1);
    }
    strcpy(bus_stops[i][1], name);
    i++;
  }
  fclose(fp);
  return;
}

void free_bus_stops() {
  for (int i = 0; i < 366; i++) {
    free(bus_stops[i][1]);
    free(bus_stops[i][0]);
    free(bus_stops[i]);
  }
  free(bus_stops);
}

int search_for_code_index(int code) {
  for (int i = 0; i < 366; i++) {
    if (atoi(bus_stops[i][0]) == code) {
      return i;
    }
  }
  return -1;
}

void match_individual_line(char* filename, int* bus_indices) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    fprintf(stderr, "File Opening Error!\n");
    exit(1);
  }
  char code[70], name[70];
  int i = 0;
  while (fscanf(fp, "%s %s", code, name) == 2) {
    int code_int = atoi(code);
    int index = search_for_code_index(code_int);
    if (index == -1) {
      fprintf(stderr, "Error!\n");
      exit(1);
    }
    bus_indices[i++] = index;
  }
  fclose(fp);
  return;
}

void match_stops_with_lines() {
  match_individual_line("./bus_stops/stops_040.txt", bus_indices_040);
  match_individual_line("./bus_stops/stops_040_return.txt", bus_indices_040_return);
  match_individual_line("./bus_stops/stops_10.txt", bus_indices_10);
  match_individual_line("./bus_stops/stops_10_return.txt", bus_indices_10_return);
  match_individual_line("./bus_stops/stops_224.txt", bus_indices_224);
  match_individual_line("./bus_stops/stops_224_return.txt", bus_indices_224_return);
  match_individual_line("./bus_stops/stops_250.txt", bus_indices_250);
  match_individual_line("./bus_stops/stops_550.txt", bus_indices_550);
  match_individual_line("./bus_stops/stops_550_return.txt", bus_indices_550_return);
  match_individual_line("./bus_stops/stops_608.txt", bus_indices_608);
  match_individual_line("./bus_stops/stops_608_return.txt", bus_indices_608_return);
}