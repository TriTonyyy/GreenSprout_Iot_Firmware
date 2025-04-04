#ifndef SERVER_API_H
#define SERVER_API_H

#include "config.h"
#include <vector>
#include "model/sensors.h"
#include "model/controls.h"

String sendSensor(String type, float value, String apiPath, bool isPost);
String sendControl(String name, bool status, float min, float max, String mode, String apiPath, bool isPost);
String sendDevice(String deviceID, std::vector<Sensor> sensors, std::vector<Control> controls, String apiPath, bool isPost);
String sendData(String payload, String apiPath, bool isPost);
StaticJsonDocument<512> getData(String apiPath);


#endif
