#include "server_api.h"
#include "control/manager/pref_manager.h"
#include "control/manager/sensor_manager.h"
#include "Arduino.h"
#include "control/manager/device_manager.h"
// String serverAddress = "https://capstone-project-iot-1.onrender.com/api/";
String serverAddress = "http://192.168.1.224:8000/api/";

String updateControlPath = "control/updateControlBy/"; 

String updateSensorPath = "sensor/updateSensorBy/"; 

String getSchedulePath = "schedule/detailScheduleBy/"; 

String createDevicePath = "device/createDevice"; 
String getDevicePath = "device/detailDeviceBy/"; 
String updateDevicePath = "device/updateDeviceBy/"; 

const unsigned long measurementInterval = 20000; 
unsigned long lastMeasurementTime = measurementInterval;


void sendSensor(String type, float value, String id, bool isPost) {
    StaticJsonDocument<128> doc;
    doc["type"] = type;
    doc["value"] = value;

    String payload;
    serializeJson(doc, payload);
    sendData(payload,updateSensorPath+deviceID+"/"+id,isPost);
}
void sendControl(Control* control, String id, bool isPost) {
    StaticJsonDocument<200> doc;
    doc["name"] = control->getName();
    doc["status"] = control->getStatus();
    doc["threshold_min"] = control->getThresholdMin();
    doc["threshold_max"] = control->getThresholdMax();
    doc["mode"] = control->getMode();
    JsonArray schedules = doc.createNestedArray("schedules");
    // Convert JSON object to a String
    String payload;
    serializeJson(doc, payload);
    sendData(payload,updateControlPath+deviceID+"/"+id,isPost);
}
void sendDevice(std::vector<Sensor*> sensors, std::vector<Control*> controls, bool isPost) {
    StaticJsonDocument<256> doc;
    doc["id_esp"] = deviceID;
    doc["name_area"] = "New Garden";
    doc["img_area"] = "";
    doc["update_at"] = "";
    doc["create_at"] = "";
    JsonArray members = doc.createNestedArray("members");
    JsonArray sensorsJS = doc.createNestedArray("sensors");
    for (Sensor* sensor : sensors) {
        JsonObject sensorJs = sensorsJS.createNestedObject();
        sensorJs["type"] = sensor->getType();
        sensorJs["value"] = sensor->getValue();
    }
    JsonArray controlsJS = doc.createNestedArray("controls");
    for (Control* control : controls) {
        JsonObject controlJs = controlsJS.createNestedObject();
        controlJs["name"] = control->getName();
        controlJs["status"] = control->getStatus();
        controlJs["threshold_min"] = control->getThresholdMin();
        controlJs["threshold_max"] = control->getThresholdMax();
        controlJs["mode"] = control->getMode();
        JsonArray schedules = controlJs.createNestedArray("schedules");
    }
    // Chuyển JSON thành chuỗi
    String payload;
    serializeJson(doc, payload);
    Serial.println(payload);
    sendData(payload,isPost?createDevicePath:updateDevicePath+deviceID,isPost);
}

void sendData(String payload, String apiPath, bool isPost) {
    HTTPClient http;
    http.begin(serverAddress + apiPath);
    http.addHeader("Content-Type", "application/json");

    Serial.println("Sending request to: " + serverAddress + apiPath);
    
    // Send HTTP request (POST or PUT)
    int httpResponseCode = isPost ? http.POST(payload) : http.PUT(payload);
    
    if (httpResponseCode > 0) {
        Serial.printf("HTTP Response Code: %d\n", httpResponseCode);
        String response = http.getString();
        Serial.println("Response: " + response);

        // Parse JSON response
        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, response);

        if (error) {
            Serial.print("Failed to parse JSON: ");
            Serial.println(error.f_str());
        }
    } else {
        Serial.printf("Error in sending data: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
}

StaticJsonDocument<512> getData(String apiPath) {
    HTTPClient http;
    
    http.begin(serverAddress + apiPath);
    http.addHeader("Content-Type", "application/json");
    
    Serial.println("Requesting data from: " + serverAddress + apiPath);
    
    int httpResponseCode = http.GET();  // Send GET request
    StaticJsonDocument<512> doc;
    
    if (httpResponseCode == 200 || httpResponseCode == 404) {
        Serial.printf("HTTP Response Code: %d\n", httpResponseCode);
        String responseData = http.getString();
        Serial.println("Response: " + responseData);
        DeserializationError error = deserializeJson(doc, responseData);

        if (error) {    
            Serial.print("Failed to parse JSON: ");
            Serial.println(error.f_str());
        }
    }else{
        Serial.printf("GET Request Failed: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
    return doc;  // Return the full response as a string
}


void updateSensorToServer(std::vector<Sensor*> sensors){
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
        float avgMoisture = calAvg(moistureLabel);
        float avgStream = calAvg(streamLabel);
        float avgLuminosity = calAvg(luminosityLabel);
        float avgTemperature = calAvg(temperatureLabel);
        float avgHumidity = calAvg(humidityLabel);

        // Send average data to the server
        sendSensor(streamLabel, avgStream,deviceID+"/"+sensors[0]->getId(),false);
        sendSensor(humidityLabel, avgHumidity,deviceID+"/"+sensors[1]->getId(),false);
        sendSensor(moistureLabel, avgMoisture,deviceID+"/"+sensors[2]->getId(),false);
        sendSensor(luminosityLabel, avgLuminosity,deviceID+"/"+sensors[3]->getId(),false);
        sendSensor(temperatureLabel, avgTemperature,deviceID+"/"+sensors[4]->getId(),false);
        
        resetSensorsData();
    }
}
