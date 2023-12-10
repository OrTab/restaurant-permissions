#include <stdio.h>
#include <stdbool.h>
#include "permissions.h"
#include "roles.h"

bool hasPermission(int userPermissions, int permissionValue)
{
    return (userPermissions & permissionValue) != 0;
}

int main()
{
    int userPermissionsInput;
    printf("\n*** Welcome to The Restaurant Of Permissions ***\n\n");
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

    bool isOwner = userPermissions == OWNER_ROLE;
    bool canProcessPayments = hasPermission(userPermissions, PROCESS_PAYMENTS);
    bool isWaiter = hasPermission(userPermissions, WAITER_ROLE);

    printf("Can process payments: %d\n", canProcessPayments);
    printf("Is waiter: %d\n", isWaiter);

    if (isWaiter)
    {
        if (hasPermission(userPermissions, VIEW_SALES_REPORTS))
        {
            printf("Should never reach here");
        }
        else
        {
            printf("Waiter shouldn\'t view sales report");
        }
    }

    return 0;
}
