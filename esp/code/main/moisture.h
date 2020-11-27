#ifndef MOISTURE_H
#define MOISTURE_H

#include "hw.h"
#include "Adafruit_seesaw.h"

int setup_moisture_sensor(Adafruit_seesaw *sensor);

uint16_t read_moisture(Adafruit_seesaw *sensor);
#endif
