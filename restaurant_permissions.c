#include <stdio.h>
#include <stdbool.h>
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

int main()
{
    int userPermissionsInput;
    int userAction;
    struct UserData *users = (struct UserData *)malloc((NUMBER_OF_USERS * sizeof(struct UserData)));
    if (users == NULL)
    {
        perror("Memory allocation failed.\n");
        return 1;
    }
    printf("\n*** Welcome to The Restaurant Of Permissions ***\n\n");
    printf("What you want to do?\n1. Add user\nAny other action press any digit above 1\n");
    scanf("%d", &userAction);
    if (userAction == 1)
    {
        char nameBuffer[100];
        printf("Please enter name: ");
        scanf(" %[^\n]", nameBuffer);
        char *userName = malloc(strlen(nameBuffer) + 1);
        strcpy(userName, nameBuffer);
        populateNewUser(users, userName, strlen(nameBuffer));
        FILE *file = fopen("users.txt", "w");
        if (file == NULL)
        {
            perror("Error opening file");
            return 1;
        }
        fprintf(file, "ID\t\tName\t\t\tName length\tPermissions\n");

        for (int i = 0; i < NUMBER_OF_USERS; i++)
        {
            fprintf(file, "%d\t\t%s\t\t\t%d\t\t\t%d\n", users[i].id, users[i].name, users[i].nameLength, users[i].permissions);
        }

        fclose(file);
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
