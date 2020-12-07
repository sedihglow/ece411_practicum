#include "comm.h"
#include "user.h"

int main(void)
{
    serv_info_s *serv_info = NULL;
    char *user_input = NULL;

    serv_info = com_init_serv_info();
    if (!serv_info)
        errExit("main: failed to init serv_info");

    /* fill serv_info struct and initialize connection to server */
    init_client_comm(serv_info);

    
    for ( ;; ) {
        display_clear();
        display_welcome();
        display_menu();
        display_prompt();

        /* get user input */
        user_input = get_user_input();
        user_input[MENU_IN_LEN] = '\0';
        if (strcmp(user_input, STR_EXIT) == 0)
            break;

        exec_user_input(serv_info, user_input);
        free(user_input);
    }
    
    free(user_input);
    com_free_serv_info(serv_info);

}
