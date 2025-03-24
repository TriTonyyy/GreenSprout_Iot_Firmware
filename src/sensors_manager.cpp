#include "sensors_manager.h"

int moisturePin = 36;
int rainPin = 39;
int lightPin = 34;
int waterFlowPin = 35;

DHT dht(DHTPIN, DHTTYPE);

void initSensors() {
    dht.begin();
    pinMode(moisturePin, INPUT);
    pinMode(rainPin, INPUT);
    pinMode(lightPin, INPUT);
    pinMode(waterFlowPin, INPUT);
}

float readSoilMoisture() {
    return map(analogRead(moisturePin), 4095, 0, 0, 100);
}

float readRainSensor() {
    return map(analogRead(rainPin), 4095, 0, 0, 100);
}

float readLightSensor() {
    return map(analogRead(lightPin), 4095, 0, 0, 100);
}

float readTemperature() {
    return dht.readTemperature();
}

float readHumidity() {
    return dht.readHumidity();
}
