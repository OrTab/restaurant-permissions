#include <stdio.h>
#include <stdlib.h>
#include "headers/user/user.h"
#include "headers/permissions/restaurant_permissions_functions.h"

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
