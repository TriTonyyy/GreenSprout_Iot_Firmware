#ifndef PREF_MANAGER_H
#define PREF_MANAGER_H
#include <Preferences.h>

// Declare global variables
extern Preferences preferences;

// Declare functions for gathering data, calculating average, and sending data
void initPreferences();
void storeData(String name, float value);
float calAvg(String name);
void resetData(String name);
#endif