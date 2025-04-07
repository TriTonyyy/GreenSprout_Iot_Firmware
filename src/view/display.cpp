#include "display.h"
void initLed()
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