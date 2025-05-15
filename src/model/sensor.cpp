#include "sensor.h"
#include "control/manager/pref_manager.h"

DHT dht(DHTPIN, DHTTYPE);
String streamLabel = "stream";
String humidityLabel = "humid";
String moistureLabel = "moist";
String luminosityLabel = "lumi";
String temperatureLabel = "temp";
String waterUsageLabel = "water";
volatile int pulseCount = 0;
unsigned long lastTime = 0;
float streamSpeed = 0.0;
void IRAM_ATTR countPulse() {
    pulseCount++;
  }
void initSensors() {
    dht.begin();
    pinMode(moisturePin, INPUT_PULLUP );
    // pinMode(rainPin, INPUT);
    pinMode(luminosityPin, INPUT_PULLUP );
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
        streamSpeed = (pulseCount * 60.0) / 5880.0;
        float waterThisSecond = (streamSpeed / 60.0); // L/min → L/sec
        float totalWaterUsage = preferences.isKey(waterUsageLabel.c_str()) ? preferences.getFloat(waterUsageLabel.c_str()):0.0; 
        preferences.putFloat(waterUsageLabel.c_str(),totalWaterUsage + waterThisSecond);
        Serial.print("Total Water Usage (L): ");
        Serial.println(totalWaterUsage + waterThisSecond);
        pulseCount = 0;
        lastTime = currentTime;

        attachInterrupt(digitalPinToInterrupt(streamPin), countPulse, RISING);
        return waterThisSecond;
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