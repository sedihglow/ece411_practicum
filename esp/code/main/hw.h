#ifndef HW_H
#define HW_H

#include <Arduino.h>
#include <Wire.h>

#define I2C_ADDR 0x36
#define SCL_PIN 2
#define SDA_PIN 14

#define TR_PIN 0 //pin used in touchRead()

#define WLS_PIN 4      // water sensor
#define PUMP_PIN 5     // water pump pin
#define EXT_LED_PIN 12 // external LED

#define BAUD_RATE 115200 // serial baud rate

#define SUCCESS 0
#define FAILURE 1

#endif
