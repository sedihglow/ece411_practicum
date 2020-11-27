#include "pump.h"

void setup_pump_pin()
{
    pinMode(PUMP_PIN, OUTPUT);
}

void pump_water(int sec)
{
    int ms = (sec * 1000);
    digitalWrite(PUMP_PIN, HIGH);
    delay(ms);
    digitalWrite(PUMP_PIN, LOW);
}
