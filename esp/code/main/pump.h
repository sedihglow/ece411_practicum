#ifndef PUMP_H
#define PUMP_H

#include "hw.h"

#define DEFAULT_PUMP_TIME 5 // in seconds

void setup_pump_pin();

void pump_water(int sec);

#endif
