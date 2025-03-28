// SensorManager.h
#ifndef SENSORMANAGER_H
#define SENSORMANAGER_H

#include <Preferences.h>

// Declare global variables
extern Preferences preferences;

// Declare functions for gathering data, calculating average, and sending data
void initPreferences();
void storeSensorData(const String& sensorName, float value);
float calculateAverage(const String& sensorName);
void resetSensorData(const String& sensorName);
#endif
