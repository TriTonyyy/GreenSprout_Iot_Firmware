#include "sensors.h"


DHT dht(DHTPIN, DHTTYPE);
String streamLabel = "stream";
String humidityLabel = "humidity";
String moistureLabel = "moisture";
String luminosityLabel = "luminosity";
String temperatureLabel = "temperature";
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
Sensor::Sensor(){
    this->id = "";
    this->type = "";
    this->value = 0;
} 
Sensor::Sensor(String type, long value){
    this->id = "";
    this->type = type;
    this->value = value;
}   
String Sensor::getId() const{
    return id;
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
void Sensor::setId(String newId) {
    id = newId;
}
void Sensor::setValue(long newValue) {
    value = newValue;
}
void Sensor::updateFromJson(const StaticJsonDocument<512>& doc)
{
    if (doc.containsKey("_id") && doc.containsKey("type") && doc.containsKey("value"))
    {
        setId(doc["_id"].as<String>());
        setType(doc["type"].as<String>());
        setValue(doc["value"].as<long>());
    }
}