#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H
#include <vector>
extern String sub;
void collectSensorsData(std::vector<Sensor*>& sensors);
void resetSensorsData();
void resetSensorRpsData();
#endif