#ifndef SERVER_API_H
#define SERVER_API_H

#include "config.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

extern WiFiUDP ntpUDP;  // NTP UDP instance
extern NTPClient timeClient;  // NTP client instance
// Initialize NTP client
void initTime();

String sendSensor(String type, float value, bool status, String apiPath, bool isPost);
String sendControl(String name, bool status, float min, float max, String mode, String apiPath, bool isPost);
String sendDevice(String deviceID, String sensorID, String controlID, String apiPath, bool isPost);
String sendData(String payload, String apiPath, bool isPost);
StaticJsonDocument<512> getData(String apiPath);
bool isExit(String apiPath);


#endif
