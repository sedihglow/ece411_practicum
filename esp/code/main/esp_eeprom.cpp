#include "esp_eeprom.h"

void init_eeprom_data(data_s *init_data)
{
    strncpy(init_data->ssid, DEFAULT_SSID, SSID_SIZE); 
    strncpy(init_data->pass, DEFAULT_PASS, PASS_SIZE);
    init_data->moisture_thresh = DEFAULT_MOISTURE_THRESH;
    init_data->pump_time = DEFAULT_PUMP_TIME;
}
