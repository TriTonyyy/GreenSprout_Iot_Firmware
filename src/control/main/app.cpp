// app.cpp
#include "app.h"
#include "view/display.h"
#include "model/sensor.h"
#include "model/control.h"
#include "control/service/server_api.h"
#include "control/service/wifi_manager.h"
#include "control/service/time_manager.h"
#include "control/manager/device_manager.h"
#include "control/manager/sensor_manager.h"
#include "control/manager/control_manager.h"
#include "control/manager/pref_manager.h"
#include "control/manager/report_manager.h"


std::vector<Sensor*> sensors;
std::vector<Control*> controls;
#define LONG_PRESS_TIME 2000
unsigned long buttonPressStart = 0;
bool buttonHeld = false;
void appSetup() {
    Serial.begin(9600);
    connectWiFi();
    initTime();
    initSensors();
    initControls();
    initPreferences();
    initDisplay();
    pinMode(wifiPin,INPUT_PULLUP);
    deviceID = getDeviceID();
    loadOrCreateDeviceConfig(sensors,controls);
}

void clearWifi(){
    int wifiState = digitalRead(wifiPin);

    if (wifiState == LOW) {
        if (!buttonHeld) {
            buttonHeld = true;
            buttonPressStart = millis();
        } else if (millis() - buttonPressStart > LONG_PRESS_TIME) {
            Serial.println("Nút được giữ lâu, reset WiFi");
            resetWiFi();
        }
    } else {
        buttonHeld = false;
    }
}
void appLoop() {
    clearWifi();
    updateTime();
    receiveControlsData(deviceID, controls);
    updateControlsBehave(controls,sensors);
    collectSensorsData(sensors);
    updateSensorToServer(sensors);
    updateReportToServer();
}