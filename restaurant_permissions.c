#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "permissions.h"
#include "roles.h"

#define NUMBER_OF_USERS 10
#define USERS_FILE_PATH "users.txt"
#define MAX_NAME_LENGTH 100
#define MAX_PERMISSION_NAME_LENGTH 30

struct UserData
{
    unsigned id;
    unsigned int permissions;
    char *name;
    unsigned int nameLength;
};

struct Permission
{
    unsigned int value;
    const char name[MAX_PERMISSION_NAME_LENGTH];
};

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

#define LENGTH_OF_PERMISSIONS sizeof(permissionsList) / sizeof(permissionsList[0])

bool hasPermission(int userPermissions, int permissionValue)
{
    return (userPermissions & permissionValue) != 0;
}

int viewPermissions(struct UserData *users)
{
    char userNameInput[MAX_NAME_LENGTH];
    printf("Please enter your name: ");
    scanf(" %[^\n]", userNameInput);
    int userId = -1;
    for (int i = 0; i < NUMBER_OF_USERS; i++)
    {
        if (strcmp(users[i].name, userNameInput) == 0)
        {
            userId = i;
            break;
        }
    }
    if (userId == -1)
    {

        printf("Couldn\'t find you!");
        return 1;
    }
    else
    {
        unsigned int userPermissions = users[userId].permissions;
        unsigned int userPermissionsCounter = 0;
        for (int i = 0; i < LENGTH_OF_PERMISSIONS; i++)
        {
            struct Permission currentPermission = permissionsList[i];
            if (userPermissions > 0 && hasPermission(userPermissions, currentPermission.value))
            {
                if (userPermissionsCounter == 0)
                {
                    printf("Nice! you can ");
                }
                else
                {
                    printf(" and ");
                }
                printf("%s", currentPermission.name);
                userPermissionsCounter++;
            }
        }

        return 0;
    }
}

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
        fprintf(file, "%u\t\t\t%u\t\t\t\t%u\t\t\t\t\t%s\n", users[i].id,
                users[i].nameLength, users[i].permissions, users[i].name);
    }

    fclose(file);
    return 0;
}

void populateNewUser(struct UserData *users, char *userName, unsigned int nameLength, unsigned int permissions)
{
    int lastId = -1;
    for (int i = NUMBER_OF_USERS - 1; i >= 0; i--)
    {
        if (users[i].nameLength > 0)
        {
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

    char nameBuffer[MAX_NAME_LENGTH];
    printf("Please enter name: ");
    scanf(" %[^\n]", nameBuffer);
    if (strlen(nameBuffer) == 0)
    {
        printf("Name cannot be empty!");
        return 1;
    }
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
        for (int i = 0; i < LENGTH_OF_PERMISSIONS; i++)
        {

            if ((permissions > 0) && hasPermission(permissionsList[i].value, permissions))
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
    printf("2. View my permissions\n");
    printf("Selection: ");
    scanf("%d", &userAction);
    if (userAction == 1)
    {
        addUser(users);
    }
    else
    {
        if (viewPermissions(users) == 1)
        {
            return 1;
        }
    }
    freeUsers(users);
    return 0;
}
