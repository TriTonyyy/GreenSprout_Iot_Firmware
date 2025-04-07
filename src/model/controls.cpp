#include "controls.h"
#include "control/server_api.h"
void initControls()
{
    pinMode(pumpPin, OUTPUT);
    pinMode(fanPin, OUTPUT);
    pinMode(lightPin, OUTPUT);

    digitalWrite(pumpPin, HIGH);
    digitalWrite(fanPin, HIGH);
    digitalWrite(lightPin, HIGH);
}


Control::Control(String name, bool status, int threshold_min, int threshold_max, String mode, int pin)
{
    this->name = name;
    this->pin = pin;
    this->status = status;
    this->threshold_min = threshold_min;
    this->threshold_max = threshold_max;
    this->mode = mode;
    this->is_running = false;
    this->schedules_id = {};

    // Initialize the pin as output
    pinMode(pin, OUTPUT);
    digitalWrite(pin, status ? LOW : HIGH); // Assuming LOW = on, HIGH = off for relay
}

// Getters
String Control::getName() const
{
    return name;
}

std::vector<String> Control::getSchedulesID() const
{
    return schedules_id;
}

bool Control::getStatus() const
{
    return status;
}

int Control::getThresholdMin() const
{
    return threshold_min;
}

int Control::getThresholdMax() const
{
    return threshold_max;
}

String Control::getMode() const
{
    return mode;
}

// Setters
void Control::setStatus(bool newStatus)
{
    status = newStatus;
    digitalWrite(pin, status ? LOW : HIGH); // LOW = on, HIGH = off for relay
}

void Control::setThresholdMin(int min)
{
    threshold_min = min;
}

void Control::setThresholdMax(int max)
{
    threshold_max = max;
}

void Control::setMode(String newMode)
{
    mode = newMode;
}

void Control::setSchedulesID(std::vector<String> ids)
{
    schedules_id = ids;
}

// Control methods
void Control::turn(bool isOn)
{
    setStatus(isOn);
    is_running = isOn;
    if(isOn){
        // Serial.println("Turning "+getName()+" on...");
        // controlLed(0,255,0);
    }else{
        // Serial.println("Turning "+getName()+" off...");
        // controlLed(255,0,0);
        // delay(1000);
        // controlLed(0,0,0);
    }
    // sendControl(name,status,threshold_min,threshold_max,mode,updateControlPath+control_id,false);

}

void Control::toggle()
{
    setStatus(!status);
    is_running = status;
    Serial.println(status ? "Turning "+name+" on..." : "Turning "+name+" off...");
}

// JSON serialization
String Control::toJson() const
{
    String json = "{";
    json += "\"name\":\"" + name + "\",";
    json += "\"status\":" + String(status ? "true" : "false") + ",";
    json += "\"threshold_min\":" + String(threshold_min) + ",";
    json += "\"threshold_max\":" + String(threshold_max) + ",";
    json += "\"mode\":\"" + mode + "\"";
    json += "}";
    return json;
}

// Update method for threshold-based control
void Control::update(float value)
{
    if (mode == "manual")
    {
        if(status && !is_running){
            turn(true);
        }else if(!status && is_running){
            turn(false);
        }
    }  
    else if (mode == "schedule")
    {
        // getData(getSchedulePath)
        // Placeholder for schedule-based control (implement as needed)
    }
    else if (mode == "threshold")
    {
        if (value < threshold_min)
        {
            // Start the pump if value is below the minimum threshold
            if (!is_running)
            {
                turn(true);
            }
        }
        else if (value >= threshold_max)
        {
            // Stop the pump only when the value reaches or exceeds the maximum threshold
            if (is_running)
            {
                turn(false);
            }
        }
        // Note: If threshold_min <= value < threshold_max, the pump continues running
        // until value >= threshold_max, ensuring it runs until max is reached
    }
}

// Update from API data
void Control::updateFromApi(const StaticJsonDocument<512> &doc)
{
    if (doc.containsKey("name") && doc.containsKey("status") &&
        doc.containsKey("threshold_min") && doc.containsKey("threshold_max") &&
        doc.containsKey("mode") && doc.containsKey("_id") && doc.containsKey("schedules"))
    {
        setStatus(doc["status"].as<bool>());
        setThresholdMin(doc["threshold_min"].as<int>());
        setThresholdMax(doc["threshold_max"].as<int>());
        setMode(doc["mode"].as<String>());
        // Serial.println(doc["schedules"].as<String>().as<JsonArray>());
        std::vector<String> schedules_id;
        Serial.println(doc["schedules"].as<String>());
        for (size_t i = 0; i < doc["schedules"].size(); i++)
        {
            schedules_id.push_back(doc["schedules"][i]["scheduleId"].as<String>());
        }
        setSchedulesID(schedules_id);
    }
}

