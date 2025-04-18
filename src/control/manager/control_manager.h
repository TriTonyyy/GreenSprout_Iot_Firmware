#ifndef CONTROL_MANAGER_H
#define CONTROL_MANAGER_H
#include <vector>
#include "model/control.h"
#include "model/sensor.h"
void receiveControlsData(String deviceID, std::vector<Control*>& controls);
void updateControlsBehave(std::vector<Control*>& controls,std::vector<Sensor*> sensors);
#endif