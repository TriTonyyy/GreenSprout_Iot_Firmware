#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;  // NTP UDP instance
NTPClient timeClient;  // NTP client instance
// Initialize NTP client
void initTime();
void updateTime();
String getDateTime();
String getTime();
String getWeekDay();