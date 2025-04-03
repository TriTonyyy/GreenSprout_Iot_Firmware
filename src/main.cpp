#include "config.h"
#include "wifi_manager.h"
#include "sensors.h"
#include "controls_manager.h"
#include "server_api.h"
#include "sensor_manager.h"
#include "time_manager.h"
#include <vector>
const unsigned long measurementInterval = 20000; 
unsigned long lastMeasurementTime = measurementInterval;


// String rainCoverName = "rain";
Control* waterControl = new Control((int)pumpPin);
Control* lightControl = new Control((int)pumpPin);
std::vector<String> idSensorsList;
std::vector<String> idControlsList;

void setup() {
    Serial.begin(9600);
    connectWiFi();
    initTime();
    initSensors();
    initControls();
    initPreferences();  // Initialize Preferences
    deviceID = "0";
    // deviceID = getDeviceID();
    StaticJsonDocument<512> responseData = getData(getDevicePath+deviceID);
    if (!responseData.containsKey("data")) {    
        // Create sensors and controls here if necessary
        idSensorsList.push_back(sendSensor(streamLabel, 0, createSensorPath, true));
        idSensorsList.push_back(sendSensor(humidityLabel, 0, createSensorPath, true));
        idSensorsList.push_back(sendSensor(moistureLabel, 0, createSensorPath, true));
        idSensorsList.push_back(sendSensor(luminosityLabel, 0, createSensorPath, true));
        idSensorsList.push_back(sendSensor(temperatureLabel, 0, createSensorPath, true));

        idControlsList.push_back(sendControl("water", false, 30, 80, "manual",createControlPath,true));
        idControlsList.push_back(sendControl("light", false, 30, 80, "manual",createControlPath,true));

        sendDevice(deviceID ,idSensorsList,idControlsList,createDevicePath,true);
    }else{
        for (size_t i = 0; i < responseData["data"]["sensors"].size(); i++) {
            String sensorId = responseData["data"]["sensors"][i]["sensorId"].as<String>();
            idSensorsList.push_back(sensorId);
        }
        for (size_t i = 0; i < responseData["data"]["controls"].size(); i++) {
            String controlId = responseData["data"]["controls"][i]["controlId"].as<String>();
            idControlsList.push_back(controlId);
        }
    }
}
void receiveControlData(){
    if(idControlsList.size() == 2){
        waterControl->updateFromApi(getData(getControlPath+idControlsList[0]));
        lightControl->updateFromApi(getData(getControlPath+idControlsList[1]));
    }
}
void collectData(){
    float moisture = readMoisture();
    float luminosity = readLuminosity();
    float temperature = readTemperature();
    float humidity = readHumidity();
    float stream = readStream();
    // Store the data in Preferences if it's valid
    if (!isnan(moisture)) {
        storeSensorData(moistureLabel, moisture);
        waterControl->update(moisture);
    }
    if (!isnan(luminosity)) {
        storeSensorData(luminosityLabel, luminosity);
        // lightControl->update(luminosity);
    }
    if (!isnan(stream)) {
        storeSensorData(streamLabel, stream);
    }
    if (!isnan(temperature) && !isnan(humidity)) {
        storeSensorData(temperatureLabel, temperature);
        storeSensorData(humidityLabel, humidity);
    }
}
void resetStoreData(){
    resetSensorData(moistureLabel);
    resetSensorData(luminosityLabel);
    resetSensorData(streamLabel);
    resetSensorData(temperatureLabel);
    resetSensorData(humidityLabel);
}
void sendToServer(){
    // Calculate and send average data to server every hour
    unsigned long currentMillis = millis();
    if (currentMillis - lastMeasurementTime >= measurementInterval) {  // 1 hour (3600000 ms)
        lastMeasurementTime = currentMillis;
        
        // Name key
        String moistureKey = moistureLabel+"_c";
        String streamKey = streamLabel+"_c";
        String luminosityKey = luminosityLabel+"_c";
        String temperatureKey = temperatureLabel+"_c";
        String humidityKey = humidityLabel+"_c";

        // Calculate the averages for each sensor
        float avgMoisture = calculateAverage(moistureLabel);
        float avgStream = calculateAverage(streamLabel);
        float avgLuminosity = calculateAverage(luminosityLabel);
        float avgTemperature = calculateAverage(temperatureLabel);
        float avgHumidity = calculateAverage(humidityLabel);

        // Send average data to the server
        sendSensor(streamLabel, avgStream,updateSensorPath+idSensorsList[0],false);
        sendSensor(humidityLabel, avgHumidity,updateSensorPath+idSensorsList[1],false);
        sendSensor(moistureLabel, avgMoisture,updateSensorPath+idSensorsList[2],false);
        sendSensor(luminosityLabel, avgLuminosity,updateSensorPath+idSensorsList[3],false);
        sendSensor(temperatureLabel, avgTemperature,updateSensorPath+idSensorsList[4],false);
        
        resetStoreData();
    }
}



void loop() {
    // Collect sensor data
    receiveControlData();
    collectData();
    sendToServer();
    resetStoreData();
}

