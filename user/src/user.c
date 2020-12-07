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

    input = CALLOC_ARRAY(char, len);
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

void display_welcome()
{
    printf("Welcome to the smart irrigator.");
}

void display_menu()
{
    printf("Select a number below" 
           "%d. change moisture threshold"
           "%d. change water pump time"
           "%d. exit program",
           (int)CHANGE_MOIST_THRESH, (int)CHANGE_PUMP_TIME, (int)EXIT);
}

void display_prompt()
{
    printf(">> ");
    fflush(stdout);
}

void display_clear()
{
    int count = 0;
    int i = 0;
    
    for (i = 0; count < NL_CLR; ++i)
        printf("\n");
    fflush(stdout);
}

