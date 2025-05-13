#include "wifi_manager.h"
#include <WiFiManager.h>
#include "esp_wifi.h"
#include <Arduino.h>
#include "control/manager/device_manager.h"
WiFiManager wifiManager;
void initDisplay()
{
    pinMode(ledBPin, OUTPUT);
    pinMode(ledGPin, OUTPUT);
    pinMode(ledRPin, OUTPUT);
}

void setColor(int red, int green, int blue)
{
    analogWrite(ledRPin, red);
    analogWrite(ledGPin, green);
    analogWrite(ledBPin, blue);
}
void connectWiFi() {
    Serial.println(WiFi.RSSI()+" dBm");
    String last4Digits = deviceID.substring(deviceID.length() - 4);
    WiFi.mode(WIFI_STA);
    String wifiName = "Green Sprout - " + last4Digits;

    
    setColor(255,0,0);
    if (!wifiManager.autoConnect(wifiName.c_str())) {
        Serial.println("Failed to connect. Restarting...");
        delay(1000);
        ESP.restart();
    }
    Serial.println("Connected to WiFi!");
    setColor(0,255,0);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    Serial.println(WiFi.RSSI()+" dBm");
}
void resetWiFi() {
    Serial.println("Resetting WiFi settings...");
    wifiManager.resetSettings(); // Clear stored WiFi credentials
    delay(1000);
    ESP.restart(); // Restart the ESP to apply changes
}
