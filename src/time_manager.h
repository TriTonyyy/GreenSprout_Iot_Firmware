#include "config.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

extern WiFiUDP ntpUDP;  // NTP UDP instance
extern NTPClient timeClient;  // NTP client instance
// Initialize NTP client
void initTime();
String getCurrentTime();
String getCurrentWeekDay();