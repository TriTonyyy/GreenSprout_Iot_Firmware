#include "wifi_manager.h"
#include "esp_wifi.h"

WiFiManager wifiManager;

void connectWiFi() {
    esp_wifi_set_max_tx_power(84);
    WiFi.setSleep(false);
    WiFi.mode(WIFI_STA);
    if (!wifiManager.autoConnect("ESP32_Setup")) {
        Serial.println("Failed to connect. Restarting...");
        delay(1000);
        ESP.restart();
    }
    Serial.println("Connected to WiFi!");
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);

    // Khởi động server
    server.begin();
}
