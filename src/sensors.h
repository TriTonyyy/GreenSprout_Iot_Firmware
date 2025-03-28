#ifndef SENSORS_H
#define SENSORS_H

#include "config.h"
#include <DHT.h>

// Chân cảm biến
#define DHTPIN 4
#define DHTTYPE DHT11

extern int moisturePin;
extern int rainPin;
extern int lightPin;
extern int waterFlowPin;

// DHT sensor
extern DHT dht;

void initSensors();
float readSoilMoisture();
float readRainSensor();
float readLightSensor();
float readTemperature();
float readHumidity();

#endif
