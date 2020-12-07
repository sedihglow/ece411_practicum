#include "comm.h"
#include "user.h"

int main(void)
{
    serv_info_s *serv_info = NULL;
    char *user_input = NULL;
    uint8_t tx[] = "testing";

    serv_info = com_init_serv_info();
    if (!serv_info)
        errExit("main: failed to init serv_info");

    /* fill serv_info struct and initialize connection to server */
    init_client_comm(serv_info);


   // send_to_server(serv_info->sockfd, tx, sizeof("testing"), NO_FLAGS);
   
    display_welcome();
    
    for ( ;; ) {
        display_clear();
        display_menu();
        display_prompt();

        /* get user input */
        user_input = get_user_input();
        if (user_input[0] == EXIT)
            break;

    }
}
