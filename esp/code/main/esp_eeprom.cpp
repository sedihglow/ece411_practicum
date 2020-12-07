#include "esp_eeprom.h"

void init_eeprom_data(data_s *init_data)
{
    strncpy(init_data->ssid, DEFAULT_SSID, SSID_SIZE); 
    strncpy(init_data->pass, DEFAULT_PASS, PASS_SIZE);
    init_data->moisture_thresh = DEFAULT_MOISTURE_THRESH;
    init_data->pump_time = DEFAULT_PUMP_TIME;
}

void change_mthresh(data_s *data, int num)
{
    data->moisture_thresh = num;
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.put(0, data);
    EEPROM.end();
    Serial.printf("new moisture threshold: %d\n", data->moisture_thresh);
}

void change_pump_time(data_s *data, int num)
{
    data->pump_time = num;
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.put(0, data);
    EEPROM.end();
    Serial.printf("new pump time: %d\n", data->pump_time);
}
