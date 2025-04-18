#ifndef SERVER_API_H
#define SERVER_API_H

#include <vector>
#include <HTTPClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "model/sensor.h"
#include "model/control.h"
#include "model/report.h"

extern String updateControlPath; 
extern String getControlsPath; 

extern String updateSensorPath; 

extern String createDevicePath; 
extern String getDevicePath; 
extern String updateDevicePath; 

extern String getSchedulePath; 
extern String updateSchedulePath; 

void sendReport(Report* report);
void sendSensors(std::vector<Sensor*> sensors);
void sendControls(std::vector<Control*> controls);
void updateSchedule(StaticJsonDocument<512> schedule);
void sendDevice(std::vector<Sensor*> sensors, std::vector<Control*> controls, bool isPost);
void sendData(String payload, String apiPath, bool isPost);
StaticJsonDocument<512> getData(String apiPath);
void updateSensorToServer(std::vector<Sensor*> sensors);

#endif
