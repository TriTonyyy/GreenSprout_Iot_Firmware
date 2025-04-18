#include "pref_manager.h"
#include <Arduino.h>

Preferences preferences;

// Initialize preferences to store sensor data
void initPreferences() {
    preferences.begin("sensor_data", false); 
}

void storeData(String name, float value) {
    if (isnan(value)) return; // Ignore invalid values
    String countKey = name + "_c";
    String sumKey = name + "_s";
    // Check if values exist; if not, initialize them
    if (!preferences.isKey(sumKey.c_str())) {
        preferences.putFloat(sumKey.c_str(), 0.0f);  // Initialize sum
    }
    if (!preferences.isKey(countKey.c_str())) {
        preferences.putInt(countKey.c_str(), 0);  // Initialize count
    }
    int count = preferences.getInt(countKey.c_str(), 0);
    float sum = preferences.getFloat(sumKey.c_str(), 0.0f);

    // Update sum and count
    sum += value;
    count++;

    // Store back into Preferences
    preferences.putFloat(sumKey.c_str(), sum);
    preferences.putInt(countKey.c_str(), count);
    
}

float calAvg(String name) {
    String countKey = name + "_c";
    String sumKey = name + "_s";
    float average = preferences.getFloat(sumKey.c_str(), 0) / preferences.getInt(countKey.c_str(), 0);;
    return average;
}


void resetData(String name) {
    String countKey = name + "_c";
    String sumKey = name + "_s";
    preferences.putInt(countKey.c_str(),0);
    preferences.putFloat(sumKey.c_str(),0);
}

