#include "config.h"
#include "wifi_manager.h"
#include "sensors.h"
#include "control.h"
#include "server_api.h"

void setup() {
    Serial.begin(9600);
    connectWiFi();
    initSensors();
    initControl();
}

void loop() {
    float soilMoisture = readSoilMoisture();
    float rainCover = readRainSensor();
    float lightIntensity = readLightSensor();
    float temperature = readTemperature();
    float humidity = readHumidity();

    if (!isnan(temperature) && !isnan(humidity)) {
        controlFan(temperature);
        controlPump(soilMoisture);
        sendSensorData("moisture", soilMoisture, true, updateSensorPath, false);
    }

    delay(1000);
}


