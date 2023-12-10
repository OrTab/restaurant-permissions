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
        perror("Error opening file");
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

void populateNewUser(struct UserData *users, char *userName, unsigned int nameLength)
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
}

int readUsersFromFile(struct UserData *users)
{
    bool isFileNotExist = access(USERS_FILE_PATH, F_OK) == -1;
    if (isFileNotExist)
    {
        writeUsersToFile(users);
        return 0;
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
    printf("Press digit above 1\n");
    printf("Selection: ");
    scanf("%d", &userAction);
    if (userAction == 1)
    {
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
        populateNewUser(users, userName, strlen(nameBuffer));
        if (writeUsersToFile(users) == 1)
        {

            return 1;
        }
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
