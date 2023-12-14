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
  printf(
      "\nName must be at least 1 char unless you wont be able to "
      "continue\n");
  printf("\nPlease enter your name: ");
  scanf(" %[^\n]", userNameInput);
  size_t nameLength = strlen(userNameInput);
  char *userName = malloc(nameLength + 1);
  if (userName == NULL) {
    perror("Memory allocation for userName failed.\n");
    exitProgram(true);
  }
  userName[nameLength] = '\0';
  strcpy(userName, userNameInput);
  return userName;
}

char *getEncryptedOrDecryptedString(char *str) {
  size_t length = strlen(str);
  char *returnValueStr = malloc(length + 1);
  if (returnValueStr == NULL) {
    perror("Error while allocate memory for encryptedName!");
    exitProgram(true);
  }
  size_t encryptionKeyLength = strlen(ENCRYPTION_KEY);
  for (size_t i = 0; i < length; i++) {
    char currentChar = str[i] ^ ENCRYPTION_KEY[i % encryptionKeyLength];
    returnValueStr[i] = currentChar;
  }
  returnValueStr[length] = '\0';
  return returnValueStr;
}

void writeUsersToFile() {
  FILE *file = fopen(USERS_FILE_PATH, "w");
  if (file == NULL) {
    perror("Error opening and writing users to file");
    exitProgram(true);
  }
  char *columnsNames[] = {"ID", "Name length", "Permissions", "Name"};
  size_t columnsNamesLength = 4;
  for (size_t i = 0; i < columnsNamesLength; i++) {
    char *encryptedName = getEncryptedOrDecryptedString(columnsNames[i]);
    fprintf(file, "%s%s", encryptedName,
            i < columnsNamesLength - 1 ? "\t\t" : "\n");
    free(encryptedName);
  }

  for (unsigned int i = 0; i < NUMBER_OF_USERS; i++) {
    if (!users[i].name) {
      continue;
    }
    char *encryptedName = getEncryptedOrDecryptedString(users[i].name);
    fprintf(file, "%u\t\t\t%u\t\t\t\t%u\t\t\t\t\t%s\n", users[i].id,
            users[i].nameLength, users[i].permissions, encryptedName);
    free(encryptedName);
  }
  if (fclose(file) != 0) {
    perror("Error closing file");
    exitProgram(true);
  }
}

void printPermissionActions(unsigned int permissions, unsigned int action) {
  unsigned int lengthOfPermissions = getLengthOfPermissions();
  for (unsigned int i = 0; i < lengthOfPermissions; i++) {
    bool hasCurrentPermission =
        hasPermission(permissionsList[i].value, permissions);
    if (action == 2) {
      if (hasCurrentPermission) {
        printf("%d. %s\n", i + 1, permissionsList[i].name);
      }
    } else {
      if (permissions > 0 && hasCurrentPermission) {
        // we added this permission already
        continue;
      } else {
        printf("%d. %s\n", i + 1, permissionsList[i].name);
      }
    }
  }
}

bool isPermissionAddedOrRemoved(unsigned int action, unsigned int permissions,
                                unsigned int currentPermissionValue) {
  if (action == 1) {
    if (hasPermission(permissions, currentPermissionValue)) {
      printf("You added this permission already!");
      return true;
    }
  } else {
    if (!hasPermission(permissions, currentPermissionValue)) {
      printf("You removed this permission already!");
      return true;
    }
  }
  return false;
}

bool isSelectionOutOfRange(int currentPermission) {
  unsigned int lengthOfPermissions = getLengthOfPermissions();
  bool isOutOfRange =
      currentPermission < 0 || currentPermission > lengthOfPermissions;
  if (isOutOfRange) {
    printf("Please enter a valid permission number (1 to %u), or 0 to finish\n",
           lengthOfPermissions);
  }
  return isOutOfRange;
}

unsigned int getUpdatedPermissionValue(unsigned int permissions,
                                       unsigned int action,
                                       unsigned int currentPermissionValue) {
  if (action == 1) {
    permissions |= currentPermissionValue;
  } else {
    permissions ^= currentPermissionValue;
  }
  return permissions;
}

/**
 * Modify user permissions.
 *
 * @param userId The ID of the user to modify. Use -1 to add a new user.
 * @param action The action to perform: 1 for add, 2 for remove.
 * @return The modified user permissions.
 */
unsigned int modifyPermission(int userId, unsigned int action) {
  unsigned int permissions = userId == -1 ? 0 : users[userId].permissions;
  unsigned int lengthOfPermissions = getLengthOfPermissions();
  while (1) {
    int currentPermission = 0;
    printf("\nEnter permission number , 0 to finish\n");
    if (permissions == OWNER_ROLE && action != 2) {
      printf("Nice! seems you the owner");
      break;
    }
    printPermissionActions(permissions, action);
    scanf("%d", &currentPermission);

    if (currentPermission == 0) {
      break;
    }
    unsigned int currentPermissionValue =
        permissionsList[currentPermission - 1].value;

    if (isPermissionAddedOrRemoved(action, permissions,
                                   currentPermissionValue)) {
      continue;
    }
    if (isSelectionOutOfRange(currentPermission)) {
      continue;
    }
    permissions =
        getUpdatedPermissionValue(permissions, action, currentPermissionValue);
  }
  return permissions;
}

void editUser() {
  char *userName = getNameFromUser();
  int userId = getUserIdByName(userName);
  if (userId == -1) {
    printf("Couldn\'t find you!");
    free(userName);
    exitProgram(true);
  }

  int choice = -1;
  while (choice != 1 && choice != 2) {
    if (choice == 0) {
      exitProgram(false);
    }
    if (choice != -1) {
      printf("\nMust enter valid choice\n");
    }
    printf("Would you like to update your name or permissions? 0 to exit\n");
    printf("1. Name\n");
    printf("2. Permissions\n");
    printf("Selection: ");
    scanf("%d", &choice);
  }
  if (choice == 1) {
    char *updatedName = getNameFromUser();
    free(users[userId].name);
    users[userId].name = updatedName;
    users[userId].nameLength = strlen(updatedName);
    writeUsersToFile();
    printf("%s updated successfully!", updatedName);
  } else if (choice == 2) {
    choice = -1;
    while (choice != 1 && choice != 2) {
      if (choice == 0) {
        exitProgram(false);
      }
      if (choice != -1) {
        printf("\nMust enter valid choice\n");
      }
      printf("Would you like remove or add permission? 0 to exit\n");
      printf("1. Add\n");
      printf("2. Remove\n");
      printf("Selection: ");
      scanf("%d", &choice);
    }
    unsigned int permissions = modifyPermission(userId, choice);
    users[userId].permissions = permissions;
    writeUsersToFile();
    printf("%s updated successfully!", userName);
  }
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
  int firstChar = fgetc(file);
  if (firstChar == EOF) {
    fclose(file);
    return writeUsersToFile();
  }
  while (fgetc(file) != '\n') {
    // Continue reading characters until a newline is found to skip file
    // header
  }
  for (unsigned int i = 0; i < NUMBER_OF_USERS; i++) {
    fscanf(file, "%u %u %u", &users[i].id, &users[i].nameLength,
           &users[i].permissions);

    unsigned int nameLength = users[i].nameLength;
    if (nameLength > 0) {
      char name[nameLength + 1];
      fscanf(file, " %[^\n]", name);
      users[i].name = getEncryptedOrDecryptedString(name);
      printf("after decrypt %s - \n", users[i].name);
    }
  }
  fclose(file);
}

void addUser() {
  char *userName = getNameFromUser();
  unsigned int permissions = modifyPermission(-1, 1);
  populateNewUser(userName, permissions);
  writeUsersToFile();
  printf("\n%s added successfully!\n", userName);
}
