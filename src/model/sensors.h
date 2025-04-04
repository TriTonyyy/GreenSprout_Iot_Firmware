#ifndef SENSORS_H
#define SENSORS_H

#include "config.h"
#include <DHT.h>

// Chân cảm biến
#define DHTPIN 4
#define DHTTYPE DHT11
#define moisturePin 36
#define streamPin 35
#define luminosityPin 34


// DHT sensor
extern DHT dht;


void initSensors();
float readMoisture();
// float readRainSensor();
float readLuminosity();
float readStream();
float readTemperature();
float readHumidity();
class Sensor{
    private:
        String type;
        long value;
    public:
        Sensor(String type, long value);

        String getType() const;
        long getValue() const;

        void setType(String newType) ;
        void setValue(long newValue) ;
        void updateFromJson(const StaticJsonDocument<512> &doc);
};

#endif
