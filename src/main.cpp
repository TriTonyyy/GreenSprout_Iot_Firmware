#include "config.h"
#include "wifi_manager.h"
#include "sensors.h"
#include "controls_manager.h"
#include "server_api.h"
#include "sensor_manager.h"

unsigned long lastMeasurementTime = 0;
const unsigned long measurementInterval = 300000; 

String moistureName = "moisture";
String rainCoverName = "rain";
String lightIntensityName = "light";
String temperatureName = "temp";
String humidityName = "humidity";
Control* pumpControl = new Control((int)pumpPin);


void setup() {
    Serial.begin(9600);
    connectWiFi();
    initSensors();
    initControls();
    initPreferences();  // Initialize Preferences
    deviceID = getDeviceID();
    StaticJsonDocument<512> responseData = getData(getDevicePath+deviceID);
    if (!responseData.containsKey("data")) {
        // Create sensors and controls here if necessary
        soilMoistureSensorID = sendSensor("moisture", 0, true, createSensorPath, true);
        waterControlID = sendControl("water", true, 30, 80, "manual",createControlPath,true);
        sendDevice(deviceID ,soilMoistureSensorID,waterControlID,createDevicePath,true);
    }else{
        soilMoistureSensorID = responseData["data"]["sensors"][0]["sensorId"].as<String>();
        waterControlID = responseData["data"]["controls"][0]["controlId"].as<String>();
    }
}
void receiveControlData(){
    pumpControl->updateFromApi(getData(getControlPath+waterControlID));
    
}
void collectData(){
    float soilMoisture = readSoilMoisture();
    float rainCover = readRainSensor();
    float lightIntensity = readLightSensor();
    float temperature = readTemperature();
    float humidity = readHumidity();
    

    Serial.println("Sensor Readings:");
    Serial.print("Soil Moisture: ");
    Serial.print(soilMoisture);
    Serial.println("%");
    Serial.print("Rain Cover: ");
    Serial.print(rainCover);
    Serial.println("%");
    Serial.print("Light Intensity: ");
    Serial.print(lightIntensity);
    Serial.println("%");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println("°C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
    // Store the data in Preferences if it's valid
    if (!isnan(soilMoisture)) {
        storeSensorData(moistureName, soilMoisture);
        pumpControl->update(temperature);
    }
    if (!isnan(rainCover)) {
        storeSensorData(rainCoverName, rainCover);
    }
    if (!isnan(lightIntensity)) {
        storeSensorData(lightIntensityName, lightIntensity);
    }
    if (!isnan(temperature) && !isnan(humidity)) {
        storeSensorData(temperatureName, temperature);
        storeSensorData(humidityName, humidity);
    }
}
void resetStoreData(){
    resetSensorData(moistureName);
    resetSensorData(rainCoverName);
    resetSensorData(lightIntensityName);
    resetSensorData(temperatureName);
    resetSensorData(humidityName);
}
void sendToServer(){
    // Calculate and send average data to server every hour
    unsigned long currentMillis = millis();
    if (currentMillis - lastMeasurementTime >= 100000) {  // 1 hour (3600000 ms)
        lastMeasurementTime = currentMillis;
        
        // Calculate the averages for each sensor
        String moistureKey = moistureName+"_count";
        String rainCoverKey = rainCoverName+"_count";
        String lightIntensityKey = lightIntensityName+"_count";
        String temperatureKey = temperatureName+"_count";
        String humidityKey = humidityName+"_count";
        float avgSoilMoisture = calculateAverage(moistureName);
        float avgRainCover = calculateAverage(rainCoverName);
        float avgLightIntensity = calculateAverage(lightIntensityName);
        float avgTemperature = calculateAverage(temperatureName);
        float avgHumidity = calculateAverage(humidityName);

        // Send average data to the server
        sendSensor("moisture", avgSoilMoisture, true,updateSensorPath+soilMoistureSensorID,false);
        // sendDataToServer("rain_cover", avgRainCover);
        // sendDataToServer("light_intensity", avgLightIntensity);
        // sendDataToServer("temperature", avgTemperature);
        // sendDataToServer("humidity", avgHumidity);
        
        resetStoreData();
    }
}



void loop() {
    // Collect sensor data
    receiveControlData();
    collectData();
    sendToServer();
    resetStoreData();
    // Delay before the next loop
    delay(1000);  // Adjust this delay based on your required data gathering frequency
}

