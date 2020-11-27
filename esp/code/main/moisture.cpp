#include "moisture.h"

int setup_moisture_sensor(Adafruit_seesaw *sensor)
{
    Wire.begin(SCL_PIN, SDA_PIN);
    if (!sensor->begin(I2C_ADDR)) {
        Serial.println("ERROR sensor not found");
        return FAILURE;
    } else {
        Serial.print("sensor started, version: ");
        Serial.println(sensor->getVersion(), HEX);
        return SUCCESS;
    }
}

uint16_t read_moisture(Adafruit_seesaw *sensor)
{
    uint16_t read = sensor->touchRead(TR_PIN);
    return read;
}
