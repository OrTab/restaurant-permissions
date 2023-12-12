#ifndef RESTAURANT_PERMISSIONS_FUNCTIONS_H_
#define RESTAURANT_PERMISSIONS_FUNCTIONS_H_
#include "../user/user.h"

int viewPermissions();
void freeUsers();
int readUsersFromFile();
int addUser();

#endif