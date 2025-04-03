#ifndef SENSORS_H
#define SENSORS_H

#include "config.h"
#include <DHT.h>

// Chân cảm biến
#define DHTPIN 4
#define DHTTYPE DHT11

extern int moisturePin;
// extern int rainPin;
extern int luminosityPin;
extern int streamPin;

// DHT sensor
extern DHT dht;

void initSensors();
float readMoisture();
// float readRainSensor();
float readLuminosity();
float readStream();
float readTemperature();
float readHumidity();

#endif
