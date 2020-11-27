#include "water_sensor.h" 

void setup_water_sensor()
{
    pinMode(WLS_PIN, INPUT);
}

int read_water_sensor()
{
    return digitalRead(WLS_PIN); 
}
