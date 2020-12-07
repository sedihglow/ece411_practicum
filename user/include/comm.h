#ifndef COMM_H
#define COMM_H

#include "utility_sys.h"

#define NO_FLAGS 0  /* used for functions where no flag argument is used. */
#define COM_ESP_ADDR    "192.168.1.45"
#define COM_ESP_LEN     sizeof(COM_ESP_ADDR)
#define COM_ESP_PORT    4210 /* port listening on esp */
#define COM_NET_DOMAIN  AF_INET        /* network domain we are using. IPV4 */
#define COM_SOCK_TYPE   SOCK_DGRAM /* udp socket */
#define COM_IP_PROTOCOL 0              /* Default for type in socket() */

#define IO_BUFF 256

typedef struct server_info {
    in_addr_t addr;      /* network binary of server address */
    char *dot_addr;      /* dotted representation of IP address */
    in_port_t port;      /* port used at IP address, network ordered */
    int domain;          /* AF_INET or AF_INET6 */
    int sock_type;       /* type of socket, socket() definition. */
    int pcol;            /* Protocol argument used in socket() */
    int sockfd;          /* socket file descriptior */
    struct sockaddr_in *socket_info; /* socket API struct, IPV4 */
} serv_info_s;

static inline ssize_t socket_transmit(int sockfd, uint8_t *tx, 
                                      size_t len, int flags)
{
    ssize_t sent;            /* number of bytes written to socket */
    size_t  remaining = len; /* number of bytes left to write */

    sent = send(sockfd, tx, remaining, flags);
    if (_usrUnlikely(sent == FAILURE)) {
        err_msg("socket_transmit: send() failed");
        return FAILURE;
    }

    /* in case there was something not written, try again */
    remaining -= sent;
    tx        += sent; 

    return (len - remaining);
}

static inline ssize_t socket_receive(int sockfd, uint8_t *rx, 
                                     size_t len, int flags)
{
    ssize_t received = 1;    /* bytes read from a socket, non-EOF init */
    size_t  remaining = len; /* bytes still in buffer */

    received = recv(sockfd, rx, remaining, flags);
    if (received == FAILURE) {
        err_msg("socket_recieve: recv() failed");
        return FAILURE;
    } 

    remaining -= received;
    rx        += received;
    
    return (len - remaining);
} 

serv_info_s* com_init_serv_info(void);
int connect_to_server(serv_info_s *serv_info);
int init_client_comm(serv_info_s *serv_info);
ssize_t send_to_server(int sockfd, uint8_t *tx, size_t len, int flags);
ssize_t receive_from_server(int sockfd, uint8_t *rx, size_t len, int flags);
void com_free_serv_info(serv_info_s *dest);

#endif
