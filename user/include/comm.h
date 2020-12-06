#ifndef COMM_H
#define COMM_H

#include "utility_sys.h"

#define COM_ESP_ADDR   "192.168.1.42"
#define COM_ESP_LEN    sizeof(COM_ESP_ADDR)
#define COM_ESP_PORT   50059 /* port listening on esp */
#define COM_NET_DOMAIN  AF_INET        /* network domain we are using. IPV4 */
#define COM_SOCK_TYPE   SOCK_DGRAM /* udp socket */
#define COM_IP_PROTOCOL 0              /* Default for type in socket() */

#endif
