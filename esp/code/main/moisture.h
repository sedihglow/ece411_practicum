#ifndef MOISTURE_H
#define MOISTURE_H

#include "hw.h"
#include "Adafruit_seesaw.h"

#define DEFAULT_MOISTURE_THRESH 900 // default low end. any lower then water

int setup_moisture_sensor(Adafruit_seesaw *sensor);

uint16_t read_moisture(Adafruit_seesaw *sensor);
#endif
