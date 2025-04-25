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
#define LONG_PRESS_TIME 2000 // 2 giây
unsigned long buttonPressStart = 0;
bool buttonHeld = false;
// const int pwmPin = 21; // Nối tới Gate MOSFET
// const int channel = 0;
// const int freq = 5000;
// const int resolution = 8;
void appSetup() {
    Serial.begin(9600);
    connectWiFi();
    initTime();
    initSensors();
    initPreferences();
    initDisplay();
    pinMode(buttonPin,INPUT_PULLUP);
    // deviceID = "9999"; 
    deviceID = getDeviceID();
    loadOrCreateDeviceConfig(sensors,controls);
    // ledcSetup(channel, freq, resolution);
    // ledcAttachPin(pwmPin, channel);
}

void appLoop() {
    // Serial.println(map(analogRead(rotatePin),0,4095,0,100));
    int buttonState = digitalRead(buttonPin);

    if (buttonState == LOW) {
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
    // Serial.println(analogRead(streamPin));
    // ledcWrite(channel, 128);
    updateTime();
    receiveControlsData(deviceID, controls);
    updateControlsBehave(controls,sensors);
    collectSensorsData(sensors);
    updateSensorToServer(sensors);
    updateReportToServer();
}
