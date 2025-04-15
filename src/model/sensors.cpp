#include "sensors.h"


DHT dht(DHTPIN, DHTTYPE);
String streamLabel = "stream";
String humidityLabel = "humidity";
String moistureLabel = "moisture";
String luminosityLabel = "luminosity";
String temperatureLabel = "temperature";
volatile int pulseCount = 0;
unsigned long lastTime = 0;
float streamSpeed = 0.0;
void IRAM_ATTR countPulse() {
    pulseCount++;
  }
void initSensors() {
    dht.begin();
    pinMode(moisturePin, INPUT);
    // pinMode(rainPin, INPUT);
    pinMode(luminosityPin, INPUT);
    pinMode(streamPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(streamPin), countPulse, RISING);

}

float readMoisture() {
    return map(analogRead(moisturePin), 4095, 0, 0, 100);
}

float readLuminosity() {
    return map(analogRead(luminosityPin), 4095, 0, 0, 100);
}
float readStream() {
    unsigned long currentTime = millis();
    if (currentTime - lastTime >= 1000) { // mỗi giây
        detachInterrupt(digitalPinToInterrupt(streamPin));

        // Chuyển đổi xung thành L/min
        // Dựa theo thông số có thể là: 5.5 hoặc 7.5 pulses/second = 1 L/min
        streamSpeed = pulseCount / 98.0;

        pulseCount = 0;
        lastTime = currentTime;

        attachInterrupt(digitalPinToInterrupt(streamPin), countPulse, RISING);
        return streamSpeed;
    }
    return 0;
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