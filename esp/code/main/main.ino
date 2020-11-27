#include "hw.h"
#include "moisture.h"
#include "external_led.h"
#include "water_sensor.h"
#include "pump.h"

Adafruit_seesaw moisture_sensor; // moisture sensor class

int moisture_thresh = DEFAULT_MOISTURE_THRESH;
uint16_t moisture_read = 0;

int pump_time = DEFAULT_PUMP_TIME; // in seconds
int water_read = 0;

int led_blink = HIGH;

void setup()
{
    int ret = SUCCESS;

    Serial.begin(BAUD_RATE);
    Serial.print('\n'); // newline for garbage terminal produces on boot

    // setup external led pin to output
    setup_led_out();

    // setup i2c moisture sensor
    ret = setup_moisture_sensor(&moisture_sensor);
    if (ret) {
        Serial.println("ERROR  moisture sensor not found");
    } else {
        Serial.println("Success, moisture sensor found");
    }

    // setup water sensor as input
    setup_water_sensor();
    Serial.println("water sensor pin setup");

    // setup pump pin as output
    setup_pump_pin();
    Serial.println("pump pin setup");
}

void loop()
{
     
    set_led(&led_blink);

    moisture_read = read_moisture(&moisture_sensor); 
    Serial.print("moisture reading: ");
    Serial.println(moisture_read);

    water_read = read_water_sensor();
    if (!water_read)
        Serial.println("water level low");
    
    // if there is still water in the tank and moisture is low
    if (water_read && moisture_read < moisture_thresh) {
        Serial.print("water being pumped for ");
        Serial.print(pump_time);
        Serial.println(" seconds");

        // start water pump for x seconds
        pump_water(pump_time);
    }

    delay(5000);
}
