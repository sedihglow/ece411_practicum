#ifndef USER_H
#define USER_H

#include "utility_sys.h"

#define NL_CLR 50 // num of newlines to print to clear screen
#define IN_BUFF_SIZE 256

#define CHANGE_MOIST_THRESH  1
#define CHANGE_PUMP_TIME 2
#define EXIT 3

char* get_user_input();
void clear_stdin();

void display_welcome();
void display_menu();
void display_prompt();
void display_clear();

#endif
