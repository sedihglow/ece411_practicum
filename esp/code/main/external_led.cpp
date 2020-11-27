#include "external_led.h"

void setup_led_out()
{
    pinMode(EXT_LED_PIN, OUTPUT);
}


void set_led(int set_led)
{
    if (set_led == HIGH) {
        digitalWrite(EXT_LED_PIN, HIGH); 
    } else {
        digitalWrite(EXT_LED_PIN, LOW);
    }
}
