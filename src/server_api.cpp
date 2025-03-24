#include "server_api.h"

void sendSensorData(String type, float value, bool status, String apiPath, bool isPost) {
    HTTPClient http;
    String payload = "{";
    payload += "\"type\": \"" + type + "\",";
    payload += "\"value\": " + String(value) + ",";
    payload += "\"status\": " + String(status);
    payload += "}";

    http.begin(serverAddress + apiPath);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = isPost ? http.POST(payload) : http.PUT(payload);
    if (httpResponseCode > 0) {
        Serial.printf("HTTP Response Code: %d\n", httpResponseCode);
        Serial.println(http.getString());
    } else {
        Serial.printf("Error in sending data: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
}
