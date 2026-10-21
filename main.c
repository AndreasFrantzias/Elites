#include <stdio.h>
#include <stdlib.h>

void homepage() {
  printf("You are at the homepage of the OASA app.\n");
  printf("Choose an option:\n");
  printf("Bus Lines (Press 1) | Arrivals (Press 2) | YOUR NEW FEATURE(S) (Press 3(, 4, 5, ...)) \n");
  int choice;
  scanf("%d", &choice);
  if (choice<1) {
    printf("Wrong Input.");
    // try again
    homepage();
  } else {
    // switch
  }
  return ;
}

int main() {
  // text of action
  homepage();

  return 0;


}






