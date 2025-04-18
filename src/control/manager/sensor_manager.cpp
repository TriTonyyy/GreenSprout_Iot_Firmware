
#include "model/sensor.h"
#include "sensor_manager.h"
#include "../manager/pref_manager.h"

String sub= "_rb";

void collectSensorsData(std::vector<Sensor*>& sensors){
    float stream = readStream();
    float humidity = readHumidity();
    float moisture = readMoisture();
    float luminosity = readLuminosity();
    float temperature = readTemperature();

    sensors[0]->setValue(stream);
    sensors[1]->setValue(humidity);
    sensors[2]->setValue(moisture);
    sensors[3]->setValue(luminosity);
    sensors[4]->setValue(temperature);

    // Store the data in Preferences if it's valid
    if (!isnan(moisture)) {
        storeData(moistureLabel, moisture);
        storeData(moistureLabel+sub, moisture);
    }
    if (!isnan(luminosity)) {
        storeData(luminosityLabel, luminosity);
        storeData(luminosityLabel+sub, luminosity);
    }
    if (!isnan(stream)) {
        storeData(streamLabel, stream);
        storeData(streamLabel+sub, stream);
    }
    if (!isnan(temperature) && !isnan(humidity)) {
        storeData(temperatureLabel, temperature);
        storeData(temperatureLabel+sub, temperature);
        storeData(humidityLabel, humidity);
        storeData(humidityLabel+sub, humidity);
    }
}
void resetSensorsData(){
    resetData(streamLabel);
    resetData(humidityLabel);
    resetData(moistureLabel);
    resetData(luminosityLabel);
    resetData(temperatureLabel);
}
void resetSensorRpsData(){
    resetData(streamLabel+sub);
    resetData(humidityLabel+sub);
    resetData(moistureLabel+sub);
    resetData(luminosityLabel+sub);
    resetData(temperatureLabel+sub);
}