#include <stdio.h>
// the following includes have header files you won't need to use,
// i'm using them for stuff you must ignore
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


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
  int** dep = malloc(150*sizeof(int*));
  if (!dep) {
    fprintf(stderr, "Memory Allocation Error!\n");
    exit(1);
  }
  int line=1, hour, min;
  dep[0] = malloc(2*sizeof(int));
  if (!dep[0]) {
    fprintf(stderr, "Memory Allocation Error!\n");
    exit(1);
  }
  dep[0][0] = 1;
  dep[0][1] = 0;
  while (fscanf(fp, "%d:%d", &hour, &min) == 2) {
    dep[line] = malloc(2*sizeof(int));
    if (!dep[line]) {
      fprintf(stderr, "Memory Allocation Error!\n");
      exit(1);
    }
    dep[line][0] = hour;
    dep[line++][1] = min;
  }
  dep[line] = malloc(2*sizeof(int));
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
  departures = malloc(11*sizeof(int**));
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
  return ;
}

void free_departures() {
  for (int i=0; i<11; i++) {
    int help = departures[i][0][1];
    for (int j=0; j<=help; j++) {
      free(departures[i][j]);
    }
    free(departures[i]);
  }
  free(departures);
  return ;
}

void parse_stops() {
  FILE *fp = fopen("./bus_stops/standalone_stops.txt", "r");
  if (!fp) {
    fprintf(stderr, "File Opening Error!\n");
    exit(1);
  }

  bus_stops = malloc(366*sizeof(char**));
  if (!bus_stops) {
    fprintf(stderr, "Memory Allocation Error!\n");
    exit(1);
  }
  int i = 0;
  char code[70], name[70];
  while (fscanf(fp, "%s %s", code, name) == 2) {
    bus_stops[i] = malloc(2*sizeof(char*));
    if (!bus_stops[i]) {
      fprintf(stderr, "Memory Allocation Error!\n");
      exit(1);
    }
    bus_stops[i][0] = malloc(60*sizeof(char));
    if (!bus_stops[i][0]) {
      fprintf(stderr, "Memory Allocation Error!\n");
      exit(1);
    }
    strcpy(bus_stops[i][0], code);
    bus_stops[i][1] = malloc(60*sizeof(char));
    if (!bus_stops[i][1]) {
      fprintf(stderr, "Memory Allocation Error!\n");
      exit(1);
    }
    strcpy(bus_stops[i][1], name);
    i++;
  }
  fclose(fp);
  return ;
}

void free_bus_stops() {
  for (int i = 0; i<366; i++) {
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
  return ;
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


// -- IGNORE END --

// --- HELPER FUNCTIONS ---

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
  switch (bus_choice){
    case 40:
      if (direction) return 1;
      return 0;
      break;
    case 10:
      if (direction) return 3;
      return 2;
      break;
    case 224:
      if (direction) return 5;
      return 4;
      break;
    case 250:
      return 6;
      break;
    case 550:
      if (direction) return 8;
      return 7;
      break;
    case 608:
      if (direction) return 10;
      return 9;
      break;
    case 666:
      // begotten
      exit(666);
    default:
      fprintf(stderr, "There exists no such bus line!");
      exit(1);
  }
}

void clearScreen()
{
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
        break;

  }
  return bus_indices;

}

int* find_bus_lines_for_stop(int stop_code, int *i) { 
  int* bus_indices_list = malloc(12*sizeof(int));
  if (!bus_indices_list) {
    fprintf(stderr, "Memory Allocation Error!\n");
    exit(1);
  }
  *i = 0;
  for (int j=0; j<11; j++) {
    int num = 0;
    int* bus_indices_individual = grab_bus_indices(j, &num);
    for (int k=0; k<num; k++) {
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
  int hour = localtime(&now)->tm_hour;
  int minute = localtime(&now)->tm_min;
  return {hour, minute};
}

// --- HELPER FUNCTIONS END ---

// --- FEATURE FUNCTIONS ---

// FEATURE 0: PRINT BUS LINES (GENERAL)
void print_bus_lines() {
  printf("\n---------------------------------------------------\n");
  printf("Bus Lines:\n");
  int indices[6] = {0, 2, 4, 6, 7, 9};
  for (int i=0; i < 6; i++) {
    int j = indices[i];
    printf("%s\n", bus_lines[j]);
  }
  printf("Write the number of the bus line you want to choose OR press 0 to go back.\n");
  return ;
}

// FEATURE 1: PRINT DEPARTURES
void print_departures(int bus_index) {
  printf("\n---------------------------------------------------\n");
  int i = 1;
  while (i < departures[bus_index][0][1]) {
    printf("%d:%d\n", departures[bus_index][i][0], departures[bus_index][i++][1]);
  }
  printf("Press Enter to go back to the main menu...\n");
  getchar();getc(stdin);
  clearScreen();
  return ;
}

void departures_menu() {
  clearScreen();
  print_bus_lines();
  int bus_choice;
  scanf("%d", &bus_choice);
  // go back if 0 is pressed
  if (bus_choice == 0) return ;
  int direction = 0;
  if (bus_choice != 250 && bus_choice != 666) {
    direction = find_direction();
  }
  int bus_index = index_of_bus(bus_choice, direction);
  switch (bus_index) {

  }
  clearScreen();
  print_departures(bus_index);
  return ;
}


// FEATURE 2: PRINT ARRIVALS

void individual_stop_arrivals(int stop_index) {
  clearScreen();
  int local_time[2] = grab_local_time();
  printf("\n---------------------------------------------------\n");
  printf("%s:\n", bus_stops[stop_index][1]);
  // find which bus lines pass through this stop
  int i = 0;
  int stop_code = atoi(bus_stops[stop_index][0]);
  int* bus_lines_that_pass_through_stop = find_bus_lines_for_stop(stop_code, &i);
  printf("\n- - - - - - - - - - - - - - - - - - - - - - - - -\n");
  for (int j=0; j<i; j++) {
    int bus_index = bus_lines_that_pass_through_stop[j];
    printf("%s \t in ...minutes\n", bus_lines[bus_index]);
    printf("\n- - - - - - - - - - - - - - - - - - - - - - - - -\n");
  }
  free(bus_lines_that_pass_through_stop);
  printf("Press R to refresh the arrivals OR press Enter to go back to the main menu...\n");
  char choice;
  getchar();choice = getc(stdin);
  if (choice == 'R' || choice == 'r') {
    individual_stop_arrivals(stop_index);
  }
  return ;
}

void print_arrivals(int bus_index, int* bus_indices, int num_stops) {
  clearScreen();
  printf("\n---------------------------------------------------\n");
  printf("%s:\n", bus_lines[bus_index]);
  int i = 0;
  while (i < num_stops) {
    int stop_index = bus_indices[i];
    printf("%d. %s\n", i+1, bus_stops[stop_index][1]);
    i++;
  }
  printf("\n---------------------------------------------------\n");
  printf("Enter the number of the stop you want to choose:\n");
  scanf("%d", &i);
  if (i < 1 || i > num_stops) {
    printf("Wrong Input! Try again.\n");
    print_arrivals(bus_index, bus_indices, num_stops);
  }
  int stop_index = bus_indices[i-1];
  individual_stop_arrivals(stop_index);
  clearScreen();
  return ;
}

void arrivals_menu() {
  clearScreen();
  print_bus_lines();
  int bus_choice;
  scanf("%d", &bus_choice);
  // go back if 0 is pressed
  if (bus_choice == 0) return ;
  int direction = 0;
  if (bus_choice != 250 && bus_choice != 666) {
    direction = find_direction();
  }
  int bus_index = index_of_bus(bus_choice, direction);
  switch (bus_index) {
    case 0:
      print_arrivals(bus_index, bus_indices_040, 38);
      break;
    case 1:
      print_arrivals(bus_index, bus_indices_040_return, 38);
      break;
    case 2:
      print_arrivals(bus_index, bus_indices_10, 47);
      break;
    case 3:
      print_arrivals(bus_index, bus_indices_10_return, 41);
      break;
    case 4:
      print_arrivals(bus_index, bus_indices_224, 34);
      break;
    case 5:
      print_arrivals(bus_index, bus_indices_224_return, 34);
      break;
    case 6:
      print_arrivals(bus_index, bus_indices_250, 26);
      break;
    case 7:
      print_arrivals(bus_index, bus_indices_550, 56);
      break;
    case 8:
      print_arrivals(bus_index, bus_indices_550_return, 56);
      break;
    case 9:
      print_arrivals(bus_index, bus_indices_608, 41);
      break;
    case 10:
      print_arrivals(bus_index, bus_indices_608_return, 45);
      break;
  }
  return ;
}


// FEATURE 3 -> YOUR TASK: ADD YOUR FEATURE FUNCTION HERE
void your_awesome_feature_menu() {
  clearScreen();
  printf("\n---------------------------------------------------\n");
  printf("ADD YOUR FEATURE HERE\n");
  printf("Press Enter to go back to the main menu...\n");
  getchar();getc(stdin);
  clearScreen();
  return ;
}



// --- FEATURE FUNCTIONS END ---

// function for the printing of the home page
void homepage() {
  printf("\n---------------------------------------------------\n");
  printf("You are at the homepage of the OASA app.\n");
  printf("Choose an option:\n");
  printf("Bus Departures (Press 1) | Bus Stops Arrivals (Press 2) | YOUR NEW FEATURE (Press 3) | Press 0 to exit the app. \n");
  int choice=0;
  // user-input
  scanf("%d", &choice);
  // switch statement -> choice of the user
  switch (choice) {
  case 0:
    return ;
    break;
  case 1:
    departures_menu();
    break;
  case 2:
    arrivals_menu();
    break;
  case 3:
    your_awesome_feature_menu();
    break;
  // default case -> in case the choice input is invalid
  default:
    //fflush(stdin);
    // calls the function itself again to print the above again
    printf("Wrong Input. Try again.\n");
  }
  clearScreen();
  homepage();
  // ends function and goes back to main
  return ;
}

// main function
int main() {
  // parsing -- ignore
  parse_departures();
  parse_stops();
  match_stops_with_lines();
  // calls the homepage function
  homepage();


  // freeing -- ignore
  free_departures();
  free_bus_stops();
  return 0;
}
