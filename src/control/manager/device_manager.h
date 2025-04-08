#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H
#include "model/controls.h"
#include "model/sensors.h"
#include <vector>
extern String deviceID;
String getDeviceID();
void loadOrCreateDeviceConfig(std::vector<Sensor*>& sensors, std::vector<Control*>& controls);
#endif