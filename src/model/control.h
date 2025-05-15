#ifndef CONTROL_H
#define CONTROL_H

#include <vector>
#include <ArduinoJson.h>

#define pumpPin 19
#define fanPin 21
#define lightPin 16


void initControls();

class Control {
private:
  String id;
  String name;           // Device name (e.g., "pump")
  bool status;           // Device state (true = on, false = off)
  int threshold_min;     // Minimum threshold
  int threshold_max;     // Maximum threshold
  String mode;           // Mode ("manual", "schedule", or "threshold")
  uint8_t pin;           // GPIO pin for controlling the device
  bool is_running;      // Track if the pump is currently running
  StaticJsonDocument<512> schedules;

public:
  // Constructor
  Control();

  Control(String name, bool status, float threshold_min, float threshold_max, String mode, int pin);
  // Getters
  String getName() const;
  String getId() const;
  StaticJsonDocument<512> getSchedules() const;
  bool getStatus() const;
  float getThresholdMin() const;
  float getThresholdMax() const;
  String getMode() const;

  // Setters
  void setSchedules(StaticJsonDocument<512> schedules);
  void setStatus(bool newStatus);
  void setThresholdMin(float min);
  void setThresholdMax(float max);
  void setMode(String newMode);
  void setId(String newId);
  void setName(String newName);
  void setPin(int newPin);
  // Control methods
  void toggle();
  void turn(bool isOn);
  // Update method for threshold-based control
  void update(float value);

  // Update from API data
  void updateFromJson(const StaticJsonDocument<512>& doc);
};
#endif