#ifndef CONTROL_H
#define CONTROL_H

#include "config.h"

#define pumpPin 16
#define bulbPin 19
#define fanPin 21

void initControl();
void controlFan(float temperature);
void controlPump(float soilMoisture);

#endif
