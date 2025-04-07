#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi và API


// ID thiết bị
extern String deviceID;




String getDeviceID();
#endif
