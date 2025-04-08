#ifndef CONTROL_MANAGER_H
#define CONTROL_MANAGER_H
#include <vector>
#include "model/controls.h"
#include "model/sensors.h"
void receiveControlsData(String deviceID, std::vector<Control*>& controls);
void updateControlsBehave(std::vector<Control*>& controls,std::vector<Sensor*> sensors);
#endif