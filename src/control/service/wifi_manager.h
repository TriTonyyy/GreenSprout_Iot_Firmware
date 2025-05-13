#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H
#define ledRPin 12
#define ledGPin 27
#define ledBPin 14
void initDisplay();
void setColor(int red, int green, int blue);
void connectWiFi();
void resetWiFi();
#endif
