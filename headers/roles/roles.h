#ifndef ROLES_H_
#define ROLES_H_
#include "../permissions/permissions.h"

#define OWNER_ROLE TOTAL_PERMISSIONS

#define WAITER_ROLE (READ_MENU | GETTING_ORDERS | ACCESS_KITCHEN)

#endif
