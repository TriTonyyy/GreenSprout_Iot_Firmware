#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi và API
extern String serverAddress;

extern String createControlPath; 
extern String getControlPath; 
extern String updateControlPath; 

extern String createSensorPath; 
extern String getSensorPath; 
extern String updateSensorPath; 

extern String createDevicePath; 
extern String getDevicePath; 
extern String updateDevicePath; 

// ID thiết bị
extern String deviceID;

extern String soilMoistureSensorID;
extern String lightSensorID;

extern String waterControlID;


// Khai báo server
extern AsyncWebServer server;
String getDeviceID();
#endif
