#ifndef ESP_EEPROM_H
#define ESP_EEPROM_H

#include <EEPROM.h>
#include "moisture.h" // for default values
#include "pump.h" // for default values

#define EEPROM_SIZE 512
#define UNINIT_PUMP_VALUE_THRESH 50
#define SSID_SIZE 32
#define PASS_SIZE 31
#define DEFAULT_SSID "DEFAULT_SSID" //"ASUS_88_2G" "DEFAULT_SSID"
#define DEFAULT_PASS "DEFAULT_PASS" //"goodness"

typedef struct eeprom_data {
    char ssid[SSID_SIZE];
    char pass[PASS_SIZE];
    uint16_t moisture_thresh;
    int pump_time; // in seconds, cannot be over UNINIT_PUMP_VALUE_THRESH
} data_s;

void init_eeprom_data(data_s *init_data);
#endif
