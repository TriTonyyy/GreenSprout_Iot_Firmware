#include "wifi_manager.h"

WiFiManager wifiManager;

void connectWiFi() {
    WiFi.mode(WIFI_STA);
    if (!wifiManager.autoConnect("ESP32_Setup")) {
        Serial.println("Failed to connect. Restarting...");
        delay(1000);
        ESP.restart();
    }
    Serial.println("Connected to WiFi!");
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);

    // Khởi động server
    server.begin();
}
