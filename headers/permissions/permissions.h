#ifndef PERMISSIONS_H_
#define PERMISSIONS_H_
#include "permissions_def.h"

#define MAX_NAME_LENGTH 100
#define MAX_PERMISSION_NAME_LENGTH 30
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

#endif