#ifndef PERMISSIONS

#define READ_MENU 1 << 0
#define GETTING_ORDERS 1 << 1
#define EDIT_MENU 1 << 2
#define PLACE_ORDERS 1 << 3
#define MANAGE_RESERVATIONS 1 << 4
#define VIEW_SALES_REPORTS 1 << 5
#define PROCESS_PAYMENTS 1 << 6
#define ACCESS_KITCHEN 1 << 7
#define EMPLOYEE_MANAGEMENT 1 << 8
#define VIEW_FEEDBACK 1 << 9

#define TOTAL_PERMISSIONS (                \
    (READ_MENU | EDIT_MENU) |              \
    (PLACE_ORDERS | MANAGE_RESERVATIONS) | \
    (VIEW_SALES_REPORTS) |                 \
    (PROCESS_PAYMENTS | ACCESS_KITCHEN) |  \
    (EMPLOYEE_MANAGEMENT | VIEW_FEEDBACK))

#endif