#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "permissions.h"
#include "roles.h"

struct UserData
{
    unsigned id;
    unsigned int permissions;
    char *name;
    unsigned int nameLength;
};

struct Permission
{
    int value;
    const char name[30];
};

#define NUMBER_OF_USERS 10
#define USERS_FILE_PATH "users.txt"

void freeUsers(struct UserData *users)
{
    for (int i = 0; i < NUMBER_OF_USERS; i++)
    {
        if (users[i].name)
        {
            free(users[i].name);
        }
    }
    free(users);
}

int writeUsersToFile(struct UserData *users)
{
    FILE *file = fopen(USERS_FILE_PATH, "w");
    if (file == NULL)
    {
        perror("Error opening and writing users to file");
        return 1;
    }
    fprintf(file, "ID\t\tName length\t\tPermissions\t\t\t\tName\n");

    for (int i = 0; i < NUMBER_OF_USERS; i++)
    {
        fprintf(file, "%u\t\t\t%u\t\t\t\t%u\t\t\t\t\t%s\n", users[i].id, users[i].nameLength, users[i].permissions, users[i].name);
    }

    fclose(file);
    return 0;
}

bool hasPermission(int userPermissions, int permissionValue)
{
    return (userPermissions & permissionValue) != 0;
}

void populateNewUser(struct UserData *users, char *userName, unsigned int nameLength, unsigned int permissions)
{
    int lastId = 0;
    for (int i = NUMBER_OF_USERS - 1; i >= 0; i--)
    {
        if (users[i].id != 0)
        {
            lastId = users[i].id;
            break;
        }
    }

    if (users[lastId].nameLength > 0)
    {
        lastId++;
    }
    users[lastId].name = userName;
    users[lastId].nameLength = nameLength;
    users[lastId].id = lastId;
    users[lastId].permissions = permissions;
}

int readUsersFromFile(struct UserData *users)
{
    bool isFileNotExist = access(USERS_FILE_PATH, F_OK) == -1;
    if (isFileNotExist)
    {
        return writeUsersToFile(users);
    }
    FILE *file = fopen(USERS_FILE_PATH, "r");
    while (fgetc(file) != '\n')
    {
        // Continue reading characters until a newline is found to skip file header
    }
    for (int i = 0; i < NUMBER_OF_USERS; i++)
    {
        fscanf(file, "%u %u %u", &users[i].id, &users[i].nameLength, &users[i].permissions);

        unsigned int nameLength = users[i].nameLength;
        if (nameLength > 0)
        {
            printf("name length is: %u\n", nameLength);
            users[i].name = malloc(nameLength + 1);
            if (users[i].name == NULL)
            {
                perror("Memory allocation for users failed.\n");
                return 1;
            }
            fscanf(file, " %[^\n]", users[i].name);
            printf("Name: %s\n", users[i].name);
        }
    }
    fclose(file);
    return 0;
}

int addUser(struct UserData *users)
{
    struct Permission permissionsList[] = {
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

    char nameBuffer[100];
    printf("Please enter name: ");
    scanf(" %[^\n]", nameBuffer);
    char *userName = malloc(strlen(nameBuffer) + 1);
    if (userName == NULL)
    {
        perror("Memory allocation for userName failed.\n");
        return 1;
    }
    strcpy(userName, nameBuffer);
    unsigned int permissions = 0;
    while (1)
    {
        unsigned int curentPermission = 0;
        printf("Enter permission number , 0 to finish\n");
        if (permissions == OWNER_ROLE)
        {
            printf("Nice! seems you the owner");
            break;
        }
        for (int i = 0; i < sizeof(permissionsList) / sizeof(permissionsList[0]); i++)
        {

            if ((permissions > 0) && (permissionsList[i].value & permissions) != 0)
            {
                // we added this permission already
                continue;
            }
            printf("%d. %s\n", i + 1, permissionsList[i].name);
        }
        scanf("%u", &curentPermission);
        if (curentPermission == 0)
        {
            break;
        }
        permissions |= permissionsList[curentPermission - 1].value;
    }
    populateNewUser(users, userName, strlen(nameBuffer), permissions);
    if (writeUsersToFile(users) == 1)
    {

        return 1;
    }
    printf("\n%s added successfully!\n", userName);
    return 0;
}

int main()
{
    int userPermissionsInput;
    int userAction;
    struct UserData *users = (struct UserData *)malloc((NUMBER_OF_USERS * sizeof(struct UserData)));
    if (users == NULL)
    {
        perror("Memory allocation for users failed.\n");
        return 1;
    }
    if (readUsersFromFile(users) == 1)
    {
        return 1;
    }
    printf("\n*** Welcome to The Restaurant Of Permissions ***\n\n");
    printf("What you want to do?\n");
    if (users[NUMBER_OF_USERS - 1].id != 0)
    {
        printf("Cannot add users, if you want so increase NUMBER_OF_USERS value\n");
    }
    else
    {
        printf("1. Add user\n");
    }
    printf("Selection: ");
    scanf("%d", &userAction);
    if (userAction == 1)
    {
        addUser(users);
    }
    else
    {
        printf("Please type in your permissions id: ");
        scanf("%d", &userPermissionsInput);
        if (userPermissionsInput <= 0)
        {
            printf("Invalid input, must be a positive number");
            return 0;
        }
        unsigned int userPermissions = (unsigned)userPermissionsInput;
        if (userPermissions > TOTAL_PERMISSIONS)
        {
            printf("Invalid input, maximum value is %d", TOTAL_PERMISSIONS);
            return 0;
        }
    }
    freeUsers(users);
    return 0;
}
