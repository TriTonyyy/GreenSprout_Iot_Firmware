// app.cpp
#include "app.h"
#include "model/sensors.h"
#include "model/controls.h"
#include "control/service/server_api.h"
#include "control/service/wifi_manager.h"
#include "control/service/time_manager.h"
#include "control/manager/device_manager.h"
#include "control/manager/sensor_manager.h"
#include "control/manager/control_manager.h"
#include "control/manager/pref_manager.h"
#include "view/display.h"
std::vector<Sensor*> sensors;
std::vector<Control*> controls;

void appSetup() {
    Serial.begin(9600);
    connectWiFi();
    initTime();
    initSensors();
    initControls();
    initPreferences();
    initDisplay();
    pinMode(buttonPin,INPUT_PULLUP);
    // deviceID = "10"; 
    deviceID = getDeviceID();
    loadOrCreateDeviceConfig(sensors,controls);
}

void appLoop() {
    // Serial.println(map(analogRead(rotatePin),0,4095,0,100));
    // Serial.println(digitalRead(buttonPin));
    // Serial.println(analogRead(streamPin));
    // updateTime();
    // updateControlsBehave(controls,sensors);
    // receiveControlsData(deviceID, controls);
    // collectSensorsData(sensors);
    // updateSensorToServer(sensors);
}
