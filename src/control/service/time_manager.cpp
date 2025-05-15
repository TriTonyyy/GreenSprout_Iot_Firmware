#include "time_manager.h"
#include <TimeLib.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600, 60000); 

// Initialize NTP time client
void initTime() {
    timeClient.begin();  // Start the NTP client
    while (!timeClient.update()) {
        delay(100); // chờ đến khi lấy được thời gian
    }
}
void updateTime(){
    timeClient.update();
}
// Function to get the current formatted time as a string
String getDateTime() {
    // Get epoch time from the timeClient
    unsigned long epochTime = timeClient.getEpochTime();
    
    // Convert epoch time to time variables using TimeLib
    setTime(epochTime);  // This sets the time using epoch time, so you can use day(), month(), year(), etc.
    
    // Format the date and time
    String dayStr = String(day());
    String monthStr = String(month());
    String yearStr = String(year());
    String timeStr = String(hour()) + ":" + String(minute()) + ":" + String(second());
    
    // Return the formatted date and time
    String dateTime = dayStr + "/" + monthStr + "/" + yearStr + " " + timeStr;
    
    return dateTime;
}
String getTime() {
    // Get epoch time from the timeClient
    unsigned long epochTime = timeClient.getEpochTime();
    setTime(epochTime);  // This sets the time using epoch time, so you can use day(), month(), year(), etc.
    String timeStr = String(hour()) + ":" + String(minute());
    return timeStr;
}
int getHour(){
    unsigned long epochTime = timeClient.getEpochTime();
    return hour();
}
String convertTo24Hour(String time12) {
    String ampm = time12.substring(time12.length() - 2); // Extract AM/PM
    String time = time12.substring(0, time12.length() - 3);
    int hour =  0;
    int minute =  0;
    sscanf(time.c_str(), "%d:%d", &hour, &minute);
    ampm.toUpperCase();
  
    // Adjust hour based on AM/PM
    if (ampm == "AM" && hour == 12) {
      hour = 0;  // Midnight case
    } else if (ampm == "PM" && hour != 12) {
      hour += 12;
    }
  
    char buffer[9]; // HH:MM:SS format
    sprintf(buffer, "%02d:%02d", hour, minute);
  
    return String(buffer);
  }
  
// Function to get the current formatted time as a string
String getDay() {
    unsigned long epochTime = timeClient.getEpochTime();
    // Get day of the week
    String weekDay;
    switch (weekday(epochTime)) {
        case 1: weekDay = "Sunday"; break;
        case 2: weekDay = "Monday"; break;
        case 3: weekDay = "Tuesday"; break;
        case 4: weekDay = "Wednesday"; break;
        case 5: weekDay = "Thursday"; break;
        case 6: weekDay = "Friday"; break;
        case 7: weekDay = "Saturday"; break;
    }
    return weekDay;  
}
String getPrevDay() {
    unsigned long epochTime = timeClient.getEpochTime();
    // Get day of the week
    String weekDay;
    switch (weekday(epochTime-86400)) {
        case 1: weekDay = "Sunday"; break;
        case 2: weekDay = "Monday"; break;
        case 3: weekDay = "Tuesday"; break;
        case 4: weekDay = "Wednesday"; break;
        case 5: weekDay = "Thursday"; break;
        case 6: weekDay = "Friday"; break;
        case 7: weekDay = "Saturday"; break;
    }
    return weekDay;  
}