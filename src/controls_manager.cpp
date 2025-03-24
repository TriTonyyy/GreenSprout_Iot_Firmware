#include "control.h"

void initControl() {
    pinMode(pumpPin, OUTPUT);
    pinMode(bulbPin, OUTPUT);
    pinMode(fanPin, OUTPUT);

    digitalWrite(pumpPin, HIGH);
    digitalWrite(bulbPin, HIGH);
    digitalWrite(fanPin, HIGH);
}

void controlFan(float temperature) {
    if (temperature > 30.0) {
        Serial.println("Turning on fan...");
        digitalWrite(fanPin, LOW);
    } else {
        digitalWrite(fanPin, HIGH);
    }
}

void controlPump(float soilMoisture) {
    if (soilMoisture < 40.0) {
        Serial.println("Turning on pump...");
        digitalWrite(pumpPin, LOW);
    } else {
        digitalWrite(pumpPin, HIGH);
    }
}
