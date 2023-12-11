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

int getUserIdByName(struct UserData *users, char userName[MAX_NAME_LENGTH]) {
  int userId = -1;

  for (unsigned int i = 0; i < NUMBER_OF_USERS; i++) {
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

int viewPermissions(struct UserData *users) {
  char userNameInput[MAX_NAME_LENGTH];
  printf("Please enter your name: ");
  scanf(" %[^\n]", userNameInput);

  int userId = getUserIdByName(users, userNameInput);
  if (userId == -1) {
    printf("Couldn\'t find you!");
    return 1;
  } else {
    unsigned int userPermissions = users[userId].permissions;
    if (userPermissions == 0) {
      printf("Oppss, seems you have no permissions");
      return 0;
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

    return 0;
  }
}

void freeUsers(struct UserData *users) {
  for (unsigned int i = 0; i < NUMBER_OF_USERS; i++) {
    if (users[i].name) {
      free(users[i].name);
    }
  }
  free(users);
}

int writeUsersToFile(struct UserData *users) {
  FILE *file = fopen(USERS_FILE_PATH, "w");
  if (file == NULL) {
    perror("Error opening and writing users to file");
    return 1;
  }
  fprintf(file, "ID\t\tName length\t\tPermissions\t\t\t\tName\n");

  for (unsigned int i = 0; i < NUMBER_OF_USERS; i++) {
    fprintf(file, "%u\t\t\t%u\t\t\t\t%u\t\t\t\t\t%s\n", users[i].id,
            users[i].nameLength, users[i].permissions, users[i].name);
  }

  fclose(file);
  return 0;
}

void populateNewUser(struct UserData *users, char *userName,
                     unsigned int nameLength, unsigned int permissions) {
  int lastId = -1;
  for (int i = NUMBER_OF_USERS - 1; i >= 0; i--) {
    if (users[i].nameLength > 0) {
      lastId = users[i].id;
      break;
    }
  }

  lastId++;

  users[lastId].name = userName;
  users[lastId].nameLength = nameLength;
  users[lastId].id = lastId;
  users[lastId].permissions = permissions;
}

int readUsersFromFile(struct UserData *users) {
  bool isFileNotExist = access(USERS_FILE_PATH, F_OK) == -1;
  if (isFileNotExist) {
    return writeUsersToFile(users);
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
        return 1;
      }
      fscanf(file, " %[^\n]", users[i].name);
      printf("Name: %s\n", users[i].name);
    }
  }
  fclose(file);
  return 0;
}

int addUser(struct UserData *users) {
  char nameBuffer[MAX_NAME_LENGTH];
  printf(
      "(Name must be at least 1 char unless you wont be able to "
      "continue)\nPlease enter name: ");
  scanf(" %[^\n]", nameBuffer);
  char *userName = malloc(strlen(nameBuffer) + 1);
  if (userName == NULL) {
    perror("Memory allocation for userName failed.\n");
    return 1;
  }
  strcpy(userName, nameBuffer);
  unsigned int permissions = 0;
  unsigned int lengthOfPermissions = getLengthOfPermissions();
  while (1) {
    unsigned int curentPermission = 0;
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
    scanf("%u", &curentPermission);
    if (curentPermission == 0) {
      break;
    }
    permissions |= permissionsList[curentPermission - 1].value;
  }
  populateNewUser(users, userName, strlen(nameBuffer), permissions);
  if (writeUsersToFile(users) == 1) {
    return 1;
  }
  printf("\n%s added successfully!\n", userName);
  return 0;
}
