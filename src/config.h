#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi và API
extern String serverAddress;
extern String createSensorPath;
extern String updateSensorPath;

// ID thiết bị
extern String deviceID;

// Khai báo server
extern AsyncWebServer server;

#endif
