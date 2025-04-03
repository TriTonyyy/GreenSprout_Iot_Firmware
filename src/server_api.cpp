#include "server_api.h"
#include "time_manager.h"

String sendSensor(String type, float value, String apiPath, bool isPost) {
    StaticJsonDocument<128> doc;
    doc["type"] = type;
    doc["value"] = value;

    String payload;
    serializeJson(doc, payload);
    String id = sendData(payload,apiPath,isPost);
    // Serial.println("Sensor "+type+" id: "+id);
    return id;
}
String sendControl(String name, bool status, float min, float max, String mode, String apiPath, bool isPost) {
    StaticJsonDocument<200> doc;
    doc["name"] = name;
    doc["status"] = status;
    doc["threshold_min"] = min;
    doc["threshold_max"] = max;
    doc["mode"] = mode;
    JsonArray schedules = doc.createNestedArray("schedules");
    // Convert JSON object to a String
    String payload;
    serializeJson(doc, payload);
    String id = sendData(payload,apiPath,isPost);
    // Serial.println("Control "+name+" id: "+id);

    return id;
}
String sendDevice(String deviceID, std::vector<String> idSensorsList, std::vector<String> idControlsList, String apiPath, bool isPost) {
    StaticJsonDocument<256> doc;
    doc["id_esp"] = deviceID;
    doc["name"] = "New Garden";
    doc["time"] = getCurrentTime();
    doc["status"] = true;
    JsonArray members = doc.createNestedArray("members");
    JsonArray sensors = doc.createNestedArray("sensors");
    for (String id : idSensorsList) {
        JsonObject sensor = sensors.createNestedObject();
        sensor["sensorId"] = id;
    }
    JsonArray controls = doc.createNestedArray("controls");
    for (String id : idControlsList) {
        JsonObject control = controls.createNestedObject();
        control["controlId"] = id;
    }
    // Chuyển JSON thành chuỗi
    String payload;
    serializeJson(doc, payload);
    // Serial.println(payload);
    String id = sendData(payload,apiPath,isPost);
    return id;
}

String sendData(String payload, String apiPath, bool isPost) {
    String id = "";  // Default empty ID
    HTTPClient http;
    http.begin(serverAddress + apiPath);
    http.addHeader("Content-Type", "application/json");

    // Serial.println("Sending request to: " + serverAddress + apiPath);
    
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
        } else if (doc.containsKey("data") && doc["data"].containsKey("_id")) {
            id = doc["data"]["_id"].as<String>(); // Extract _id
        } else {
            Serial.println("Response does not contain expected 'data' or '_id'");
        }
    } else {
        Serial.printf("Error in sending data: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
    return id;
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
