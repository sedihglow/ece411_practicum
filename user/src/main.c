#include "comm.h"

int main(void)
{
    serv_info_s *serv_info = NULL;
    uint8_t tx[] = "testing";

    serv_info = com_init_serv_info();
    if (!serv_info)
        errExit("main: failed to init serv_info");

    /* fill serv_info struct and initialize connection to server */
    init_client_comm(serv_info);


    send_to_server(serv_info->sockfd, tx, sizeof("testing"), NO_FLAGS);

    for ( ;; ) {
        

    }
}
