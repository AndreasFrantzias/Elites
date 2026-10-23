#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "parser.h"
#include "helpers.h"

int find_direction() {
  printf("---------------------------------------------------\n");
  printf("Choose Direction:\n Normal Direction: From Start to Terminal (Press 'N') OR Opposite Direction: From Terminal to Start (Press 'O')\n");
  char direction;
  scanf(" %c", &direction);
  if (direction == 'O' || direction == 'o') {
    return 1;
  } else if (direction == 'N' || direction == 'n') {
    return 0;
  } else {
    printf("Wrong Input! Try again.\n");
    return find_direction();
  }
}

int index_of_bus(int bus_choice, int direction) {
  // if direction == 0 -> then normal, otherwise opposite
  switch (bus_choice) {
    case 40:
      if (direction) return 1;
      return 0;
    case 10:
      if (direction) return 3;
      return 2;
    case 224:
      if (direction) return 5;
      return 4;
    case 250:
      return 6;
    case 550:
      if (direction) return 8;
      return 7;
    case 608:
      if (direction) return 10;
      return 9;
    case 666:
      fprintf(stderr, "BEGOTTEN (1989) \nThe Creation Myth brought to life, the story of no less than the violent death of God and the (re)birth of nature on a barren earth.\n https://ok.ru/video/7921556065009\n");
      exit(666);
    default:
      fprintf(stderr, "There exists no such bus line!");
      exit(1);
  }
}

void clearScreen() {
  const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
  write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

int* grab_bus_indices(int bus_index, int* num_stops) {
  int* bus_indices;
  switch (bus_index) {
    case 0:
      *num_stops = 38;
      bus_indices = bus_indices_040;
      break;
    case 1:
      *num_stops = 38;
      bus_indices = bus_indices_040_return;
      break;
    case 2:
      *num_stops = 47;
      bus_indices = bus_indices_10;
      break;
    case 3:
      *num_stops = 41;
      bus_indices = bus_indices_10_return;
      break;
    case 4:
      *num_stops = 34;
      bus_indices = bus_indices_224;
      break;
    case 5:
      *num_stops = 34;
      bus_indices = bus_indices_224_return;
      break;
    case 6:
      *num_stops = 26;
      bus_indices = bus_indices_250;
      break;
    case 7:
      *num_stops = 56;
      bus_indices = bus_indices_550;
      break;
    case 8:
      *num_stops = 56;
      bus_indices = bus_indices_550_return;
      break;
    case 9:
      *num_stops = 41;
      bus_indices = bus_indices_608;
      break;
    case 10:
      *num_stops = 45;
      bus_indices = bus_indices_608_return;
      break;
    default:
      fprintf(stderr, "There exists no such index!");
      exit(1);
  }
  return bus_indices;
}

int* find_bus_lines_for_stop(int stop_code, int *i) {
  int* bus_indices_list = malloc(12 * sizeof(int));
  if (!bus_indices_list) {
    fprintf(stderr, "Memory Allocation Error!\n");
    exit(1);
  }
  *i = 0;
  for (int j = 0; j < 11; j++) {
    int num = 0;
    int* bus_indices_individual = grab_bus_indices(j, &num);
    for (int k = 0; k < num; k++) {
      int check_bus_index = atoi(bus_stops[bus_indices_individual[k]][0]);
      if (check_bus_index == stop_code) {
        bus_indices_list[(*i)++] = j;
        break;
      }
    }
  }
  return bus_indices_list;
}

int* grab_local_time() {
  time_t now = time(NULL);
  struct tm *tm_now = localtime(&now);
  int* local_time = malloc(2 * sizeof(int));
  if (!local_time) {
    fprintf(stderr, "Memory Allocation Error!\n");
    exit(1);
  }
  local_time[0] = tm_now->tm_hour;
  local_time[1] = tm_now->tm_min;
  return local_time;
}

int calculate_arrival_time(int* localtime, int* departuretime, int arrival_index, int bus_index) {
  (void)bus_index;
  int dep_hour = departuretime[0];
  int dep_min = departuretime[1];
  int total_time = arrival_index * 2;
  while (dep_hour != localtime[0] || dep_min != localtime[1]) {
    dep_min++;
    if (dep_min == 60) {
      dep_min = 0;
      dep_hour++;
      if (dep_hour == 24) {
        dep_hour = 0;
      }
    }
    total_time--;
  }
  return total_time;
}

int* list_of_arrival_times_two_hour_window(int* localtime, int arrival_index, int bus_index) {
  int two_hours_before = localtime[0] - 2;
  int two_hours_before_min = localtime[1];
  if (two_hours_before < 0) {
    two_hours_before = 0;
    two_hours_before_min = 0;
  }
  int num_departures = departures[bus_index][0][1];
  int i = 1;
  while (i < num_departures && (two_hours_before > departures[bus_index][i][0] || (two_hours_before == departures[bus_index][i][0] && two_hours_before_min > departures[bus_index][i][1]))) {
    i++;
  }
  int end = i;
  while (end < num_departures && (departures[bus_index][end][0] < localtime[0] || (departures[bus_index][end][0] == localtime[0] && departures[bus_index][end][1] <= localtime[1]))) {
    end++;
  }
  int* arrival_times = malloc(15 * sizeof(int));
  if (!arrival_times) {
    fprintf(stderr, "Memory Allocation Error!\n");
    exit(1);
  }
  for (int k = 0; k < 15; k++) {
    arrival_times[k] = -1;
  }
  int j = 0;
  for (; i < end && j < 15; i++) {
    int arrival_time = calculate_arrival_time(localtime, departures[bus_index][i], arrival_index, bus_index);
    if (arrival_time > 0) {
      arrival_times[j++] = arrival_time;
    }
  }
  return arrival_times;
}