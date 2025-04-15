#include "wifi_manager.h"
#include <WiFiManager.h>
#include "esp_wifi.h"
#include <Arduino.h>
WiFiManager wifiManager;

void connectWiFi() {
    Serial.println(WiFi.RSSI()+" dBm");
    // esp_wifi_set_max_tx_power(84);
    // WiFi.setSleep(false);
    WiFi.mode(WIFI_STA);
    if (!wifiManager.autoConnect("ESP32_Setup")) {
        Serial.println("Failed to connect. Restarting...");
        delay(1000);
        ESP.restart();
    }
    Serial.println("Connected to WiFi!");
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
