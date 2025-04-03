#include "sensors.h"

int moisturePin = 36;
// int rainPin = 39;
int luminosityPin = 34;
int streamPin = 35;

DHT dht(DHTPIN, DHTTYPE);

void initSensors() {
    dht.begin();
    pinMode(moisturePin, INPUT);
    // pinMode(rainPin, INPUT);
    pinMode(luminosityPin, INPUT);
    pinMode(streamPin, INPUT);
}

float readMoisture() {
    return map(analogRead(moisturePin), 4095, 0, 0, 100);
}
// float readRainSensor() {
//     return map(analogRead(rainPin), 4095, 0, 0, 100);
// }

float readLuminosity() {
    return map(analogRead(luminosityPin), 4095, 0, 0, 100);
}
float readStream() {
    return map(analogRead(streamPin), 4095, 0, 0, 100);
}
float readTemperature() {
    return dht.readTemperature();
}

float readHumidity() {
    return dht.readHumidity();
}
