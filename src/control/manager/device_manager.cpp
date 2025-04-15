#include "device_manager.h"
#include "../service/server_api.h"
String deviceID;
String getDeviceID(){
    uint64_t chipId = ESP.getEfuseMac();
    char deviceID[17];
    sprintf(deviceID, "%llX", chipId);
    return String(deviceID);
}
void loadOrCreateDeviceConfig(std::vector<Sensor*>& sensors, std::vector<Control*>& controls){
    StaticJsonDocument<512> responseData = getData(getDevicePath+deviceID);
    if (!responseData.containsKey("data")) {    
        // Create sensors and controls here if necessary
        Sensor* streamSensor = new Sensor(streamLabel, 0);
        Sensor* humiditySensor = new Sensor(humidityLabel,0);
        Sensor* moistureSensor = new Sensor(moistureLabel,0);
        Sensor* luminositySensor = new Sensor(luminosityLabel,0);
        Sensor* temperatureSensor = new Sensor(temperatureLabel,0);

        sensors.push_back(streamSensor);
        sensors.push_back(humiditySensor);
        sensors.push_back(moistureSensor);
        sensors.push_back(luminositySensor);
        sensors.push_back(temperatureSensor);

        Control* waterControl = new Control("water",false,20,30,"manual",(int)pumpPin);
        Control* lightControl = new Control("light",false,20,30,"manual",(int)lightPin);
        Control* windControl = new Control("wind",false,20,30,"manual",(int)fanPin);

        controls.push_back(waterControl);
        controls.push_back(lightControl);
        controls.push_back(windControl);

        sendDevice(sensors,controls,true);
        sensors.clear();
        controls.clear();
        loadOrCreateDeviceConfig(sensors, controls);
    }else{
        for (size_t i = 0; i < responseData["data"]["sensors"].size(); i++) {
            Sensor* newSensor = new Sensor();
            newSensor->updateFromJson(responseData["data"]["sensors"][i]);
            sensors.push_back(newSensor);
        }
        for (size_t i = 0; i < responseData["data"]["controls"].size(); i++) {
            Control* newControl = new Control();
            newControl->updateFromJson(responseData["data"]["controls"][i]);
            controls.push_back(newControl);
        }
    }
}