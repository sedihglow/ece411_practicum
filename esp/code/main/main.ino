#include "hw.h"
#include "moisture.h"
#include "external_led.h"

Adafruit_seesaw moisture_sensor; // moisture sensor class

void setup()
{
    int ret = SUCCESS;

    Serial.begin(BAUD_RATE);
    Serial.println(" "); // newline for garbage terminal produces on boot

    // setup external led pin to output
    setup_led_out();

    // setup i2c moisture sensor
    ret = setup_moisture_sensor(&moisture_sensor);
    if (ret) {
        Serial.println("ERROR  moisture sensor not found");
    } else {
        Serial.println("Success, moisture sensor found");
    }

    // setup water sensor
    setup_water_sensor();
}

void loop()
{
    uint16_t moisture_read = 0;
    int water_read = 0;
     
    set_led(HIGH);

    moisture_read = read_moisture(&moisture_sensor); 
    Serial.print("moisture reading: ");
    Serial.println(moisture_read);

    water_read = read_water_sensor();
    if (!water_read)
        Serial.print("water level low");

    set_led(LOW);
    delay(1000);
}
