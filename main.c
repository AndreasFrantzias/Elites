#include <stdio.h>
// the following includes have header files you won't need to use,
// i'm using them for stuff you must ignore
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/* GRABS LOCAL TIME AND CONVERTS TO HOURS AND MINUTES

time_t now = time(NULL);
  int hour = localtime(&now)->tm_hour;
  int minute = localtime(&now)->tm_min;
*/
// GLOBAL VARIABLES
char bus_lines[6][60] = {"040 PEIRAIAS - SINTAGMA", "10 TZITZIFIES - XALANDRI", "224 KAISARIANI - EL. VENIZELOY", "250 PANEPISTIMIOUPOLI - ST. EVAGGELISMOS", "550 P. FALIRO - KIFISIA", "608 GALATSI - AKADIMIA - NEKR. ZOGRAFOY"};
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

// --- HELPER FUNCTIONS END ---

// --- FEATURE FUNCTIONS ---

// FEATURE 0: PRINT BUS LINES (GENERAL)
void print_bus_lines() {
  printf("\n---------------------------------------------------\n");
  printf("Bus Lines:\n");
  for (int i=0; i < 7; i++) {
    printf("%s\n", bus_lines[i]);
  }
  printf("Write the number of the bus line you want to choose.\n");
  printf("OR press 0 to go back.\n");
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
  clearScreen();
  print_departures(bus_index);
  return ;
}



// --- FEATURE FUNCTIONS END ---

// function for the printing of the home page
void homepage() {
  printf("\n---------------------------------------------------\n");
  printf("You are at the homepage of the OASA app.\n");
  printf("Choose an option:\n");
  printf("Bus Departures (Press 1) | Bus Stops Arrivals (Press 2) | YOUR NEW FEATURE(S) (Press 3(, 4, 5, ...)) \n");
  printf("Press 0 to exit the app.\n");
  int choice;
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
    // calls arrivals
    break;
  case 3:
    // TODO: ENTER YOUR FEATURE FUNCTION HERE
    break;
  // default case -> in case the choice input is invalid
  default:
    printf("Wrong Input. Try again.\n");
    // calls the function itself again to print the above again
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
