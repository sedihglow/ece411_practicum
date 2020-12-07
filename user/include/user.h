#ifndef USER_H
#define USER_H

#include "utility_sys.h"
#include "comm.h"

#define NL_CLR 50 // num of newlines to print to clear screen
#define IN_BUFF_SIZE 256
#define MENU_IN_LEN 1

#define CHANGE_MOIST_THRESH  1
#define CHANGE_PUMP_TIME 2
#define EXIT 3
#define STR_EXIT "3"

char* get_user_input();
void clear_stdin();
char* get_new_moist_thresh();
char* get_new_pump_time();

void exec_user_input(serv_info_s *serv_info, char *user_input);

void display_welcome();
void display_menu();
void display_prompt();
void display_clear();

#endif
