#include "Adafruit_seesaw.h"
#include <Wire.h>

#define SCL 2
#define SDA 14
#define I2C_ADDR 0x36
#define TR_PIN 0 //pin used in touchRead()

Adafruit_seesaw ss;

void setup()
{
    Serial.begin(115200);
    Serial.println("seesaw soil sensor example.");

    Wire.begin(SCL, SDA);

    if (!ss.begin(I2C_ADDR)) {
        Serial.println("ERROR seesaw not found");
        while(true);
    } else {
        Serial.print("seesaw started. version: ");
        Serial.println(ss.getVersion(), HEX);
    }
}

void loop()
{
    float tempC = ss.getTemp();
    uint16_t capRead = ss.touchRead(0);

    Serial.print("Temperature: "); Serial.print(tempC); Serial.println("*C");
    Serial.print("Capacitive: "); Serial.println(capRead);
    delay(100);
}
