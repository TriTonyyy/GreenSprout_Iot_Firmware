#ifndef SERVER_API_H
#define SERVER_API_H

#include <vector>
#include <HTTPClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "model/sensors.h"
#include "model/controls.h"


extern String updateControlPath; 

extern String updateSensorPath; 

extern String createDevicePath; 
extern String getDevicePath; 
extern String updateDevicePath; 

extern String getSchedulePath; 
extern String updateSchedulePath; 

void sendSensors(std::vector<Sensor*> sensors);
void sendControls(std::vector<Control*> controls);
void updateSchedule(StaticJsonDocument<512> schedule);
void sendDevice(std::vector<Sensor*> sensors, std::vector<Control*> controls, bool isPost);
void sendData(String payload, String apiPath, bool isPost);
StaticJsonDocument<512> getData(String apiPath);
void updateSensorToServer(std::vector<Sensor*> sensors);

#endif
