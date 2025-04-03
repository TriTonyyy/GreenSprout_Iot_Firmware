#include "time_manager.h"
#include <TimeLib.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600, 60000); 

// Initialize NTP time client
void initTime() {
    timeClient.begin();  // Start the NTP client
}
// Function to get the current formatted time as a string
String getCurrentTime() {
    if (timeClient.update()) {  // Only get time if update is successful
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
        return timeClient.getFormattedTime();  // Return the formatted time (hh:mm:ss)
    } else {
        Serial.println("Failed to get time from NTP server");
        return "00:00:00";  // Return a default value or handle the error as needed
    }
}
// Function to get the current formatted time as a string
String getCurrentWeekDay() {
    if (timeClient.update()) {  // Only get time if update is successful
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
    } else {
        Serial.println("Failed to get time from NTP server");
        return "Monday";  // Return a default value or handle the error as needed
    }
}