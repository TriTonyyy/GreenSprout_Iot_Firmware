#include "time_manager.h"
#include <TimeLib.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600, 60000); 

// Initialize NTP time client
void initTime() {
    timeClient.begin();  // Start the NTP client
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
    String timeStr = String(hour()) + ":" + String(minute()) + ":" + String(second());
    return timeStr;
}
// Function to get the current formatted time as a string
String getWeekDay() {
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