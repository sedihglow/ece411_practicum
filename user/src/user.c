#include "user.h"

char* get_user_input()
{
    char *input = NULL;
    char buff[IN_BUFF_SIZE] = {'\0'};
    size_t len = 0;

    fgets(buff, IN_BUFF_SIZE, stdin);
    len = strlen(buff) - 1;
    if (buff[len] == '\n') {
        buff[len] = '\0';
    } else {
        clear_stdin();
    }

    input = CALLOC_ARRAY(char, len+1);
    if (!input)
        errExit("user.c: failed to allocate room for input");
    
    strncpy(input, buff, len);
    
    return input;
}

void clear_stdin()
{
    char ch = '\0';
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void exec_user_input(serv_info_s *serv_info, char *menu_input)
{
    char *new_input = NULL;
    int numval = convInt(menu_input, CN_NOEXIT_ | CN_BASE_10, "numval");
    
    switch (numval) {
    case CHANGE_MOIST_THRESH:
        new_input = get_new_moist_thresh();
        send_new_moist_thresh(serv_info, new_input);
    break;
    case CHANGE_PUMP_TIME:
        new_input = get_new_pump_time();
        send_new_pump_time(serv_info, new_input);
    break;
    }
    free(new_input);
}

void display_welcome()
{
    printf("Welcome to the smart irrigator.\n\n");
}

void display_menu()
{
    printf("Select a number below\n" 
           "%d. change moisture threshold\n"
           "%d. change water pump time\n"
           "%d. exit program\n",
           (int)CHANGE_MOIST_THRESH, (int)CHANGE_PUMP_TIME, (int)EXIT);
}

void display_prompt()
{
    printf(">> ");
    fflush(stdout);
}

void display_clear()
{
    int i = 0;
    
    for (i = 0; i < NL_CLR; ++i)
        printf("\n");
    fflush(stdout);
}

char* get_new_moist_thresh()
{
    char *input = NULL;
    int val_check = 0;
    do {
        printf("\nNew moisture threshold (1-1000): ");
        if (input)
            free(input);
        input = get_user_input();
        val_check = convInt(input, CN_NOEXIT_ | CN_BASE_10, "val_check");
    } while (val_check < 1 || val_check > 1000);
    return input;
}

char* get_new_pump_time()
{
    char *input = NULL;
    int val_check = 0;
    do {
        if (input)
            free(input);
        printf("\nNew pump time in seconds (1-10): ");
        input = get_user_input();
        val_check = convInt(input, CN_NOEXIT_ | CN_BASE_10, "val_check");
    } while (val_check < 1 || val_check > 10);
    return input; 
}
