#include <stdio.h>
// the following includes have header files you won't need to use,
// i'm using them for stuff you must ignore
#include <stdlib.h>
#include <time.h>

/* GRABS LOCAL TIME AND CONVERTS TO HOURS AND MINUTES

time_t now = time(NULL);
  int hour = localtime(&now)->tm_hour;
  int minute = localtime(&now)->tm_min;
*/
// global variable of the text of the bus lines
char bus_lines[7][60] = {"040 PEIRAIAS - SINTAGMA", "10 TZITZIFIES - XALANDRI", "224 KAISARIANI - EL. VENIZELOY", "250 PANEPISTIMIOUPOLI - ST. EVAGGELISMOS", "550 P. FALIRO - KIFISIA", "608 GALATSI - AKADIMIA - NEKR. ZOGRAFOY", "E90 PEIRAIAS - PANEPISTIMIOYPOLI (SCHOLIKI EXPRESS)"};
int*** departures = NULL;

// IGNORE THESE FUNCTIONS -> IT JUST PARSES THE INFORMATION
// FOUND IN THE .TXT FILES
// -- IGNORE --

int** parse_departures(char* filename) {
  FILE *fp = fopen(filename);
  if (!fp) {
    fprintf(stderr, "File Error.\n");
  exit(1);
  }
  int** departures = malloc(150*sizeof(int*));
  if (!departures) {
    fprintf(stderr, "Memory Allocation Error!\n");
    exit(1);
  }
  int line=0, hour, min;
  while (fscanf(fp, "%d:%d", &hour, &min) == 2) {
    departures[line] = malloc(2*sizeof(int));
    if (!departures[line]) {
      fprintf(stderr, "Memory Allocation Error!\n");
      exit(1);
    }
    departures[line][0] = hour;
    departures[line++][1] = min;
  }
  fclose(fp);
  return departures;
}

char** parse_bus_stops() {
  char** buslines = malloc(14*sizeof(char*));
  if (!buslines) {
    fprintf(stderr, "Memory Allocation Error!\n");
    exit(1);
  }
  FILE *fp = fopen("./bus_stops/stops_040.txt");
  if (!fp) {
    fprintf(stderr, "File Error.\n");
    exit(1);
  }
  int stop_code;
  char stop_name[80];
  while ()
}

// -- IGNORE --

// --- FEATURE FUNCTIONS ---

// FEATURE 0: PRINT BUS LINES (GENERAL)
void print_bus_lines() {
  printf("Bus Lines:\n");
  for (int i=0; i < 7; i++) {
    printf("%s\n", bus_lines[i]);
  }
  printf("Write the number of the bus line you want to choose.\n");
  return ;
}

// FEATURE 1: PRINT DEPARTURES
void print_departures(int bus_line) {
  
}

void departures_menu() {
  print_bus_lines();
  int bus_choice;
  scanf("%d", &bus_choice);
  switch (bus_choice) {
    case 40:
      
  }
  return ;
}



// --- FEATURE FUNCTIONS END ---

// function for the printing of the home page
void homepage() {
  printf("You are at the homepage of the OASA app.\n");
  printf("Choose an option:\n");
  printf("Bus Departures (Press 1) | Bus Stops Arrivals (Press 2) | YOUR NEW FEATURE(S) (Press 3(, 4, 5, ...)) \n");
  int choice;
  // user-input
  scanf("%d", &choice);
  // switch statement -> choice of the user
  switch (choice) {
  case 1:
    // call departures function
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
    homepage();
    break;
  }
  // ends function and goes back to main
  return ;
}

// main function
int main() {
  // calls the homepage function
  homepage();

  return 0;


}






