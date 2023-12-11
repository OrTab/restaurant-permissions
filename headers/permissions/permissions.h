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

#endif