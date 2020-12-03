#ifndef IRRIGATOR_WIFI_H
#define IRRIGATOR_WIFI_H

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include "esp_eeprom.h"

#define CONNECTION_WAIT_COUNT 20
#define WAIT_DELAY 500

void setup_wifi();

/* 
 * Functions below written at
 * https://electronicsinnovation.com/change-esp8266-wifi-credentials-without-
 * uploading-code-from-arduino-ide/
 * edited and modified by James Ross
 */
bool test_wifi();
void setup_ap();
void launch_web();
void create_web_server();


#endif
