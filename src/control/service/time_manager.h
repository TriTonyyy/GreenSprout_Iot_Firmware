#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H
#include <NTPClient.h>
#include <WiFiUdp.h>
// Initialize NTP client
void initTime();
void updateTime();
String getDateTime();
String getTime();
int getHour();
String getDay();
String getPrevDay();
String convertTo24Hour(String time12);
#endif