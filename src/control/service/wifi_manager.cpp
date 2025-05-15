#include "wifi_manager.h"
#include <WiFiManager.h>
#include "esp_wifi.h"
#include <Arduino.h>
#include "control/manager/device_manager.h"
#include "control/main/app.h"

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

// WiFi event handler
void WiFiEvent(WiFiEvent_t event) {
    switch (event) {
        case SYSTEM_EVENT_STA_DISCONNECTED:
            Serial.println("WiFi disconnected! Attempting to reconnect...");
            setColor(255, 0, 0);  // Red for disconnected
            for (size_t i = 0; i < controls.size(); i++)
            {
                controls[i]->turn(false);
            }
            WiFi.reconnect();
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
            Serial.println("WiFi connected.");
            setColor(0, 255, 0);  // Green for connected
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            Serial.print("Got IP address: ");
            Serial.println(WiFi.localIP());
            break;
        default:
            break;
    }
}

void connectWiFi() {
    Serial.println(WiFi.RSSI());
    String last4Digits = deviceID.substring(deviceID.length() - 4);
    String wifiName = "Green Sprout - " + last4Digits;

    WiFi.mode(WIFI_STA);

    setColor(255, 0, 0);
    if (!wifiManager.autoConnect(wifiName.c_str())) {
        Serial.println("Failed to connect. Restarting...");
        delay(1000);
        ESP.restart();
    }
    Serial.println("Connected to WiFi!");
    setColor(0, 255, 0);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    
    // Register the WiFi event handler
    WiFi.onEvent(WiFiEvent);
}

void resetWiFi() {
    Serial.println("Resetting WiFi settings...");
    wifiManager.resetSettings(); // Clear stored WiFi credentials
    delay(1000);
    ESP.restart(); // Restart the ESP to apply changes
}
