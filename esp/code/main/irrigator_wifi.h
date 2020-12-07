#ifndef IRRIGATOR_WIFI_H
#define IRRIGATOR_WIFI_H

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include "esp_eeprom.h"

#define CONNECTION_WAIT_COUNT 20
#define WAIT_DELAY 500

#define UDP_PORT 4210
#define PACKET_BUFF_SIZE 256

#define C_MTHRESH 0x0
#define C_PUMP_TIME 0x1

void setup_wifi();

/* 
 * test_wifi, setup_ap, launch_web and create_web_server found here
 * https://electronicsinnovation.com/change-esp8266-wifi-credentials-without-
 * uploading-code-from-arduino-ide/
 * edited and modified by James Ross
 */
bool test_wifi();
void setup_ap();
void launch_web();
void create_web_server();

void handle_user_request(char *incoming_packet);
#endif
