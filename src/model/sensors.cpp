#include "sensors.h"


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

Sensor::Sensor(String type, long value){
    this->type = type;
    this->value = value;
}   
String Sensor::getType() const{
    return type;
}
long Sensor::getValue() const{
    return value;
}
void Sensor::setType(String newType) {
    type = newType;
}
void Sensor::setValue(long newValue) {
    value = newValue;
}
void Sensor::updateFromJson(const StaticJsonDocument<512> &doc)
{
    if (doc.containsKey("type") && doc.containsKey("value"))
    {
        setStatus(doc["type"].as<String>());
        setThresholdMin(doc["value"].as<long>());
    }
}