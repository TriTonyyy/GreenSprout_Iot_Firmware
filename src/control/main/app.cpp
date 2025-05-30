// app.cpp
#include "app.h"
#include "model/sensor.h"
#include "control/service/server_api.h"
#include "control/service/wifi_manager.h"
#include "control/service/time_manager.h"
#include "control/manager/device_manager.h"
#include "control/manager/sensor_manager.h"
#include "control/manager/control_manager.h"
#include "control/manager/pref_manager.h"
#include "control/manager/report_manager.h"

std::vector<Control*> controls;
std::vector<Sensor*> sensors;
#define LONG_PRESS_TIME 100
unsigned long buttonPressStart = 0;
bool buttonHeld = false;
void appSetup() {
    Serial.begin(9600);
    deviceID = getDeviceID();
    initDisplay();
    connectWiFi();
    initTime();
    initSensors();
    initControls();
    initPreferences();
    pinMode(wifiPin,INPUT_PULLUP);
    loadOrCreateDeviceConfig(sensors,controls);
}

void clearWifi(){
    int wifiState = digitalRead(wifiPin);

    if (wifiState == LOW) {
        if (!buttonHeld) {
            buttonHeld = true;
            buttonPressStart = millis();
        } else if (millis() - buttonPressStart > LONG_PRESS_TIME) {
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