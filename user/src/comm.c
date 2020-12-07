#include "comm.h"

serv_info_s* com_init_serv_info(void)
{
    serv_info_s *init;

    init = CALLOC(serv_info_s);
    if (!init) {
        err_msg("com_init_serv_info: calloc failure - serv_info_s");
        return NULL;
    }
    
    /* init dot representation address */
    init->dot_addr = CALLOC_ARRAY(char, COM_ESP_LEN);
    if (!init->dot_addr) {
        err_msg("com_init_serv_info: calloc failure - dot_addr");
        return NULL;
    }

    init->socket_info = CALLOC(struct sockaddr_in);
    if (!init->socket_info) {
        err_msg("com_init_serv_info: calloc failure - socket_info");
        return NULL;
    }
    
    return init;
}

/******************************************************************************
 * TODO: Memory not cleaned up on exit. 
 *
 * Replace errExit functions and clean up memory in irc_server before killing
 * program
 ******************************************************************************/
int init_client_comm(serv_info_s *serv_info)
{
    struct sockaddr_in *addr_tmp = serv_info->socket_info;

    /* dot to binary representation */
    if (inet_pton(COM_NET_DOMAIN, COM_ESP_ADDR, &serv_info->addr) != 1) {
        if (errno) {
            errExit("irc_client: inet_pton failed to convert IP to binary "
                    "network order");
        }
        errnumExit(EINVAL, "irc_client: Invalid network address string");
    }

    /* init serv_info  */
    serv_info->domain    = COM_NET_DOMAIN;
    serv_info->pcol      = COM_IP_PROTOCOL;
    serv_info->sock_type = COM_SOCK_TYPE;
    /* convert port to network order */
    serv_info->port = htons(COM_ESP_PORT);

    strncpy(serv_info->dot_addr, COM_ESP_ADDR, strlen(COM_ESP_ADDR));

    /* init socket_info values */
    addr_tmp->sin_family = serv_info->domain;
    addr_tmp->sin_addr.s_addr = serv_info->addr;
    addr_tmp->sin_port = serv_info->port;

    /* connect_to_server() sets serv_info->sockfd */
    if(connect_to_server(serv_info) == FAILURE)
        errExit("irc_client: Initial connection to server failed");

    return SUCCESS;
}



int connect_to_server(serv_info_s *serv_info)
{
    int ret;
    int sockfd;
    socklen_t socklen = sizeof(struct sockaddr_in);

    if (serv_info == NULL) {
        errExit("connect_to_server: server info was NULL");
        return EINVAL;
    }

    /* open socket */
    sockfd = socket(serv_info->domain, serv_info->sock_type, serv_info->pcol);
    if (_usrUnlikely(sockfd == FAILURE)) {
        /* TODO: Retry to open socket until timeout */
        err_msg("connect_to_server: Failed to open the socket()");
        return FAILURE;
    }

    /* make connection */
    ret = connect(sockfd, (struct sockaddr*)serv_info->socket_info, socklen);
    if (_usrUnlikely(ret == FAILURE)) {
        /* TODO: error handle, retry connection till timeout. */
        err_msg("connect_to_server: Failed on connect() to server");
        return FAILURE;
    }

    serv_info->sockfd = sockfd;
    return SUCCESS;
} 

/* returns number of bytes written to socket buffer, if error occures,
 * errno will be set to the appropriate error value from send(). */
ssize_t send_to_server(int sockfd, uint8_t *tx, size_t len, int flags)
{
    return socket_transmit(sockfd, tx, len, flags);
}

ssize_t receive_from_server(int sockfd, uint8_t *rx, size_t len, int flags)
{
    return socket_receive(sockfd, rx, len, flags);
}

/* msg format: C_MTHRESH | char *msg | \r */
int send_new_moist_thresh(serv_info_s *serv_info, char *mthresh)
{
    uint8_t *tx = NULL;
    int i = 0;
    size_t mthresh_len = strlen(mthresh) + 1;
    size_t tx_len = MSG_TYPE_SIZE + mthresh_len + 1;

    tx = CALLOC_ARRAY(uint8_t, tx_len);
    if (!tx)
        errExit("send_new_moist_thresh: Failed to allocate TX");
  
    /* fill in client name */
    i = 0;
    tx[i] = C_MTHRESH;
    ++i;

    strncpy((char*)(tx+i), mthresh, mthresh_len);
    i += mthresh_len;

    tx[i] = '\r';
    ++i;
   
    send_to_server(serv_info->sockfd, tx, tx_len, NO_FLAGS);
    
    free(tx);
    return SUCCESS;
}

int send_new_pump_time(serv_info_s *serv_info, char *ptime)
{
    uint8_t *tx = NULL;
    int i = 0;
    size_t ptime_len = strlen(ptime) + 1;
    size_t tx_len = MSG_TYPE_SIZE + ptime_len + 1;

    tx = CALLOC_ARRAY(uint8_t, tx_len);
    if (!tx)
        errExit("send_new_moist_thresh: Failed to allocate TX");
  
    /* fill in client name */
    i = 0;
    tx[i] = C_PUMP_TIME;
    ++i;

    strncpy((char*)(tx+i), ptime, ptime_len);
    i += ptime_len;

    tx[i] = '\r';
    ++i;
   
    send_to_server(serv_info->sockfd, tx, tx_len, NO_FLAGS);
    
    free(tx);
    return SUCCESS;
}

void com_free_serv_info(serv_info_s *dest)
{
    FREE_ALL(dest->dot_addr, dest->socket_info, dest);
}
