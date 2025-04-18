#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H
#include "model/control.h"
#include "model/sensor.h"
#include <vector>
extern String deviceID;
String getDeviceID();
void loadOrCreateDeviceConfig(std::vector<Sensor*>& sensors, std::vector<Control*>& controls);
#endif