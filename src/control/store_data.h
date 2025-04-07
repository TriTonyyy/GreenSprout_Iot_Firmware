#ifndef STORE_DATA_H
#define STORE_DATA_H
#include <Preferences.h>

// Declare global variables
extern Preferences preferences;

// Declare functions for gathering data, calculating average, and sending data
void initPreferences();
void storeData(String name, float value);
float calAvg(String name);
void resetData(String name);
#endif