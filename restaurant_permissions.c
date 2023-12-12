#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "headers/permissions/permissions.h"
#include "headers/roles/roles.h"
#include "headers/user/user.h"

const struct Permission permissionsList[] = {
    {READ_MENU, "Read Menu"},
    {GETTING_ORDERS, "Getting Orders"},
    {EDIT_MENU, "Edit Menu"},
    {PLACE_ORDERS, "Place Orders"},
    {MANAGE_RESERVATIONS, "Manage Reservations"},
    {VIEW_SALES_REPORTS, "View Sales Reports"},
    {PROCESS_PAYMENTS, "Process Payments"},
    {ACCESS_KITCHEN, "Access Kitchen"},
    {EMPLOYEE_MANAGEMENT, "Employee Management"},
    {VIEW_FEEDBACK, "View Feedback"}};

void freeUsers() {
  for (unsigned int i = 0; i < NUMBER_OF_USERS; i++) {
    if (users[i].name) {
      free(users[i].name);
    }
  }
  free(users);
}

void exitProgram(bool isFailure) {
  freeUsers();
  exit(isFailure ? EXIT_FAILURE : EXIT_SUCCESS);
}

int getUserIdByName(char *userName) {
  int userId = -1;

  for (unsigned int i = 0; i < NUMBER_OF_USERS; i++) {
    if (!users[i].name) {
      break;
    }
    if (strcmp(users[i].name, userName) == 0) {
      userId = i;
      break;
    }
  }
  return userId;
}

bool hasPermission(unsigned int userPermissions, unsigned int permissionValue) {
  return (userPermissions & permissionValue) != 0;
}

unsigned int getLengthOfPermissions() {
  return sizeof(permissionsList) / sizeof(permissionsList[0]);
}

char *getNameFromUser() {
  char userNameInput[MAX_NAME_LENGTH];
  printf("Please enter your name: ");
  scanf(" %[^\n]", userNameInput);
  char *userName = malloc(strlen(userNameInput) + 1);
  if (userName == NULL) {
    perror("Memory allocation for userName failed.\n");
    exitProgram(true);
  }
  strcpy(userName, userNameInput);
  return userName;
}

void editUser() {
  char *userName = getNameFromUser();
  int userId = getUserIdByName(userName);
  if (userId == -1) {
    printf("Couldn\'t find you!");
    free(userName);
    exitProgram(true);
  }
  printf("Hey %s!\nEdit implantation coming soon :)", userName);
}

void viewPermissions() {
  char *userName = getNameFromUser();
  int userId = getUserIdByName(userName);
  free(userName);
  if (userId == -1) {
    printf("Couldn\'t find you!");
    exitProgram(true);
  }
  unsigned int userPermissions = users[userId].permissions;
  if (userPermissions == 0) {
    printf("Oppss, seems you have no permissions");
    exitProgram(false);
  }
  unsigned int userPermissionsCounter = 0;
  unsigned int lengthOfPermissions = getLengthOfPermissions();

  for (unsigned int i = 0; i < lengthOfPermissions; i++) {
    struct Permission currentPermission = permissionsList[i];
    if (userPermissions > 0 &&
        hasPermission(userPermissions, currentPermission.value)) {
      if (userPermissionsCounter == 0) {
        printf("Nice! you can ");
      } else {
        printf(" and ");
      }
      printf("%s", currentPermission.name);
      userPermissionsCounter++;
    }
  }
}

void writeUsersToFile() {
  FILE *file = fopen(USERS_FILE_PATH, "w");
  if (file == NULL) {
    perror("Error opening and writing users to file");
    exitProgram(true);
  }
  fprintf(file, "ID\t\tName length\t\tPermissions\t\t\t\tName\n");

  for (unsigned int i = 0; i < NUMBER_OF_USERS; i++) {
    fprintf(file, "%u\t\t\t%u\t\t\t\t%u\t\t\t\t\t%s\n", users[i].id,
            users[i].nameLength, users[i].permissions, users[i].name);
  }
  fclose(file);
}

void populateNewUser(char *userName, unsigned int permissions) {
  int lastId = -1;
  for (int i = NUMBER_OF_USERS - 1; i >= 0; i--) {
    if (users[i].nameLength > 0) {
      lastId = users[i].id;
      break;
    }
  }
  lastId++;
  users[lastId].name = userName;
  users[lastId].nameLength = strlen(userName);
  users[lastId].id = lastId;
  users[lastId].permissions = permissions;
}

void readUsersFromFile() {
  bool isFileNotExist = access(USERS_FILE_PATH, F_OK) == -1;
  if (isFileNotExist) {
    return writeUsersToFile();
  }
  FILE *file = fopen(USERS_FILE_PATH, "r");
  while (fgetc(file) != '\n') {
    // Continue reading characters until a newline is found to skip file header
  }
  for (unsigned int i = 0; i < NUMBER_OF_USERS; i++) {
    fscanf(file, "%u %u %u", &users[i].id, &users[i].nameLength,
           &users[i].permissions);

    unsigned int nameLength = users[i].nameLength;
    if (nameLength > 0) {
      printf("name length is: %u\n", nameLength);
      users[i].name = malloc(nameLength + 1);
      if (users[i].name == NULL) {
        perror("Memory allocation for user name failed.\n");
        exitProgram(true);
      }
      fscanf(file, " %[^\n]", users[i].name);
      printf("Name: %s\n", users[i].name);
    }
  }
  fclose(file);
}

void addUser() {
  printf(
      "(Name must be at least 1 char unless you wont be able to "
      "continue)\n");
  char *userName = getNameFromUser();
  unsigned int permissions = 0;
  unsigned int lengthOfPermissions = getLengthOfPermissions();
  while (1) {
    int currentPermission = 0;
    printf("Enter permission number , 0 to finish\n");
    if (permissions == OWNER_ROLE) {
      printf("Nice! seems you the owner");
      break;
    }
    for (unsigned int i = 0; i < lengthOfPermissions; i++) {
      if ((permissions > 0) &&
          hasPermission(permissionsList[i].value, permissions)) {
        // we added this permission already
        continue;
      }
      printf("%d. %s\n", i + 1, permissionsList[i].name);
    }
    scanf("%d", &currentPermission);
    if (currentPermission == 0) {
      break;
    }
    if (currentPermission < 0 || currentPermission > lengthOfPermissions) {
      printf(
          "Please enter a valid permission number (1 to %u), or 0 to finish\n",
          lengthOfPermissions);
      continue;
    }
    permissions |= permissionsList[currentPermission - 1].value;
  }
  populateNewUser(userName, permissions);
  writeUsersToFile();
  printf("\n%s added successfully!\n", userName);
}
