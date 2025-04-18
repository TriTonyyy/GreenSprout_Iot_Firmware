#include "server_api.h"
#include "control/manager/pref_manager.h"
#include "control/manager/sensor_manager.h"
#include "Arduino.h"
#include "control/manager/device_manager.h"
String serverAddress = "https://capstone-project-iot-1.onrender.com/api/";
// String serverAddress = "http://192.168.1.224:8000/api/";

String updateControlPath = "control/updateControls/"; 
String getControlsPath = "control/detailControls/"; 

String updateSensorPath = "sensor/updateSensors/"; 

String getSchedulePath = "schedule/scheduleBy/"; 
String updateSchedulePath = "schedule/updateSchedule/"; 

String getReportPath = "report/detailReport/";
String updateReportPath = "report/updateReport/";

String createDevicePath = "device/createDevice"; 
String getDevicePath = "device/detailDeviceBy/"; 
String updateDevicePath = "device/updateDevice/"; 

const unsigned long measurementInterval = 20000; 
unsigned long lastMeasurementTime = measurementInterval;

void sendReport(Report* report) {
    StaticJsonDocument<518> doc;
    doc["water_usage"] = report->getWaterUsage();
    JsonArray moistureArr = doc.createNestedArray("moisture_avg");
    moistureArr.add(report->getMoistureAvg());

    JsonArray luminosityArr = doc.createNestedArray("luminosity_avg");
    luminosityArr.add(report->getLuminosityAvg());

    JsonArray temperatureArr = doc.createNestedArray("tempurature_avg"); // keep the same spelling as your schema
    temperatureArr.add(report->getTemperatureAvg());

    JsonArray humidityArr = doc.createNestedArray("humidity_avg");
    humidityArr.add(report->getHumidityAvg());

    JsonArray streamArr = doc.createNestedArray("stream_avg");
    streamArr.add(report->getStreamAvg());
    String payload;
    serializeJson(doc, payload);
    sendData(payload,updateReportPath+deviceID,false);
}
void sendSensors(std::vector<Sensor*> sensors) {
    StaticJsonDocument<518> doc;
    for (size_t i = 0; i < sensors.size(); i++)
    {
        JsonObject sensorJs = doc.createNestedObject();
        sensorJs["sensorId"] = sensors[i]->getId();
        sensorJs["type"] = sensors[i]->getType();
        sensorJs["value"] = sensors[i]->getValue();
    }
    String payload;
    serializeJson(doc, payload);
    sendData(payload,updateSensorPath+deviceID,false);
}
void sendControls(std::vector<Control*> controls) {
    StaticJsonDocument<200> doc;
    for (size_t i = 0; i < controls.size(); i++)
    {
        JsonObject controlJs = doc.createNestedObject();
        controlJs["controlId"] = controls[i]->getId();
        controlJs["name"] = controls[i]->getName();
        controlJs["status"] = controls[i]->getStatus();
        controlJs["threshold_min"] = controls[i]->getThresholdMin();
        controlJs["threshold_max"] = controls[i]->getThresholdMax();
        controlJs["mode"] = controls[i]->getMode();
    }
    
    // Convert JSON object to a String
    String payload;
    serializeJson(doc, payload);
    Serial.println(payload);
    sendData(payload,updateControlPath+deviceID,false);
}
void updateSchedule(StaticJsonDocument<512> schedule) {
    String payload;
    serializeJson(schedule, payload);
    sendData(payload,updateSchedulePath+deviceID+"/"+schedule["_id"].as<String>(),true);
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
        // Calculate the averages for each sensor
        float avgMoisture = calAvg(moistureLabel);
        float avgStream = calAvg(streamLabel);
        float avgLuminosity = calAvg(luminosityLabel);
        float avgTemperature = calAvg(temperatureLabel);
        float avgHumidity = calAvg(humidityLabel);

        sensors[0]->setValue(avgStream);
        sensors[1]->setValue(avgHumidity);
        sensors[2]->setValue(avgMoisture);
        sensors[3]->setValue(avgLuminosity);
        sensors[4]->setValue(avgTemperature);

        // Send average data to the server
        sendSensors(sensors);
        
        resetSensorsData();
    }
}
