#ifndef CONTROL_H
#define CONTROL_H

#include <vector>
#include <ArduinoJson.h>

#define pumpPin 16
#define fanPin 19
#define lightPin 21


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
  std::vector<String> schedules_id;

public:
  // Constructor
  Control();

  Control(String name, bool status, int threshold_min, int threshold_max, String mode, int pin);
  // Getters
  String getName() const;
  String getId() const;
  std::vector<String> getSchedulesID() const;
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
  void setId(String newId);

  // Control methods
  void toggle();
  void turn(bool isOn);
  // Update method for threshold-based control
  void update(float value);

  // Update from API data
  void updateFromJson(const StaticJsonDocument<512>& doc);
};
#endif