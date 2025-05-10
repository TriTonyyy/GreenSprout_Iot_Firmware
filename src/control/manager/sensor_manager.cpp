
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
Serial.print("Stream (L/min): ");
Serial.println(stream);

Serial.print("Humidity (%): ");
Serial.println(humidity);

Serial.print("Moisture (% or value): ");
Serial.println(moisture);

Serial.print("Luminosity (lux or raw): ");
Serial.println(luminosity);

Serial.print("Temperature (°C): ");
Serial.println(temperature);

   

    // Store the data in Preferences if it's valid
    if (!isnan(moisture) && moisture != 2147483648.00) {
        sensors[2]->setValue(moisture);
        storeData(moistureLabel, moisture);
        storeData(moistureLabel+sub, moisture);
    }
    if (!isnan(luminosity)  && luminosity != 2147483648.00) {
        storeData(luminosityLabel, luminosity);
        storeData(luminosityLabel+sub, luminosity);
        sensors[3]->setValue(luminosity);
    }
    if (!isnan(stream) && stream != 2147483648.00) {
        sensors[0]->setValue(stream);
        storeData(streamLabel, stream);
        storeData(streamLabel+sub, stream);
    }
    if (!isnan(temperature) && temperature != 2147483648.00 && !isnan(humidity) && humidity != 2147483648.00) {
        sensors[1]->setValue(humidity);
        sensors[4]->setValue(temperature);
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
    preferences.putFloat(waterUsageLabel.c_str(),0.0);
}