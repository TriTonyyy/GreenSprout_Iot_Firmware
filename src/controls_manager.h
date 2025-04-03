#ifndef CONTROL_H
#define CONTROL_H

#include "config.h"

#define pumpPin 16
#define bulbPin 19
#define lightPin 21

#define ledRPin 12
#define ledGPin 27
#define ledBPin 14



void initControls();
void controlLed(int red, int green, int blue);

class Control {
private:
  String name;           // Device name (e.g., "pump")
  String control_id;      // Control ID from the API
  bool status;           // Device state (true = on, false = off)
  int threshold_min;     // Minimum threshold
  int threshold_max;     // Maximum threshold
  String mode;           // Mode ("manual", "schedule", or "threshold")
  uint8_t pin;           // GPIO pin for controlling the device
  bool is_running;      // Track if the pump is currently running
  std::vector<String> schedules_id;

public:
  // Constructor
  Control(int pin);
  // Getters
  String getName() const;
  std::vector<String> getSchedulesID() const;
  String getControlId() const;
  bool getStatus() const;
  int getThresholdMin() const;
  int getThresholdMax() const;
  String getMode() const;

  // Setters
  void setSchedulesID(std::vector<String> ids);
  void setStatus(bool newStatus);
  void setThresholdMin(int min);
  void setThresholdMax(int max);
  void setMode(String newMode);
  void setControlId(String controlId);

  // Control methods
  void toggle();
  void turn(bool isOn);
  // JSON serialization
  String toJson() const;

  // Update method for threshold-based control
  void update(float value);

  // Update from API data
  void updateFromApi(const StaticJsonDocument<512>& doc);
};
#endif