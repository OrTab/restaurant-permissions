#ifndef RESTAURANT_PERMISSIONS_FUNCTIONS_H_
#define RESTAURANT_PERMISSIONS_FUNCTIONS_H_
#include "../user/user.h"

int viewPermissions(struct UserData *users);
void freeUsers(struct UserData *users);
int readUsersFromFile(struct UserData *users);
int addUser(struct UserData *users);

#endif