#include <stdio.h>
#include <stdlib.h>

#include "headers/permissions/restaurant_permissions_functions.h"
#include "headers/user/user.h"

struct UserData *users = NULL;

int main() {
  int userAction;
  users = malloc((NUMBER_OF_USERS * sizeof(struct UserData)));
  if (users == NULL) {
    perror("Memory allocation for users failed.\n");
    return 1;
  }
  readUsersFromFile();
  printf("\n*** Welcome to The Restaurant Of Permissions ***\n\n");
  printf("What you want to do?\n");
  if (users[NUMBER_OF_USERS - 1].id != 0) {
    printf("Cannot add users, if you want so increase NUMBER_OF_USERS value\n");
  } else {
    printf("1. Add user\n");
  }
  printf("2. Edit user\n");
  printf("3. View my permissions\n");
  printf("Selection: ");
  scanf("%d", &userAction);
  if (userAction == 1) {
    addUser();
  } else if (userAction == 2) {
    editUser();
  } else if (userAction == 3) {
    viewPermissions();
  }
  freeUsers();
  return 0;
}
