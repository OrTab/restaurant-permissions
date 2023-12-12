#ifndef USER_H_
#define USER_H_
#include "../permissions/permissions.h"

#define NUMBER_OF_USERS 10
#define USERS_FILE_PATH "users.txt"

struct UserData {
  unsigned id;
  unsigned int permissions;
  char *name;
  unsigned int nameLength;
};

extern struct UserData *users;

#endif
