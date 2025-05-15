#include "model/control.h"
#include "control/service/server_api.h"
#include "control/manager/device_manager.h"
#include "control/service/time_manager.h"
void initControls()
{
    pinMode(pumpPin, OUTPUT);
    pinMode(fanPin, OUTPUT);
    pinMode(lightPin, OUTPUT);
    digitalWrite(pumpPin,LOW);
    digitalWrite(fanPin,LOW);
    digitalWrite(lightPin,LOW);
}
Control::Control()
{
    this->id = "";
    this->name = "";
    this->pin = 0;
    this->status = false;
    this->threshold_min = 20.00;
    this->threshold_max = 30.00;
    this->mode = "manual";
    this->is_running = false;
    this->schedules;
}

Control::Control(String name, bool status, float threshold_min, float threshold_max, String mode, int pin)
{
    this->id = "";
    this->name = name;
    this->pin = pin;
    this->status = status;
    this->threshold_min = threshold_min;
    this->threshold_max = threshold_max;
    this->mode = mode;
    this->is_running = false;
    this->schedules;
    digitalWrite(pin, status ? HIGH : LOW); // Assuming LOW = on, HIGH = off for relay
}

// Getters
String Control::getName() const
{
    return name;
}
String Control::getId() const
{
    return id;
}

StaticJsonDocument<512> Control::getSchedules() const
{
    return schedules;
}

bool Control::getStatus() const
{
    return status;
}

float Control::getThresholdMin() const
{
    return threshold_min;
}

float Control::getThresholdMax() const
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
    is_running = newStatus;
    digitalWrite(pin, status ? HIGH: LOW); 
}

void Control::setName(String newName)
{
    name = newName;
}
void Control::setThresholdMin(float min)
{
    threshold_min = min;
}

void Control::setThresholdMax(float max)
{
    threshold_max = max;
}

void Control::setMode(String newMode)
{
    mode = newMode;
}
void Control::setId(String newId)
{
    id = newId;
}
void Control::setSchedules(StaticJsonDocument<512> newSchedules)
{
    schedules = newSchedules;
}
void Control::setPin(int newPin){
    pin = newPin;
}

// Control methods
void Control::turn(bool isOn)
{
    setStatus(isOn);
    std::vector<Control*> controls;
    controls.push_back(this);
    sendControls(controls);

}

void Control::toggle()
{
    setStatus(!status);
    is_running = status;
}


// Update method for threshold-based control
void Control::update(float value)
{
    if (mode == "manual")
    {
        if (status && !is_running)
            turn(true);
        else if (!status && is_running)
            turn(false);
    }  
    else if (mode == "schedule")
    {
        String currentDay = getDay();
        String currentTime = getTime();
        int curH, curM;
        sscanf(currentTime.c_str(), "%d:%d", &curH, &curM);
        int currentSeconds = curH * 3600 + curM * 60;
        for (size_t i = 0; i < schedules.size(); i++)
        {
            if (schedules[i]["status"])
            {
                String startTime = convertTo24Hour(schedules[i]["startTime"]);
                int duration = schedules[i]["duration"].as<int>();
                int startH, startM, startS;
                sscanf(startTime.c_str(), "%d:%d", &startH, &startM);

                int startSeconds = startH * 3600 + startM * 60;
                int endSeconds = startSeconds + duration;
                
                bool isPassDay = false;
                if (endSeconds >= 86400)  // next-day overflow
                {
                    endSeconds -= 86400;
                    isPassDay = true;
                }

                JsonArray repeatDays = schedules[i]["repeat"].as<JsonArray>();
                if(repeatDays.size() > 0){
                    for (size_t j = 0; j < repeatDays.size(); j++)
                    {
                        String day = repeatDays[j];
                        bool matchToday = (!isPassDay && day == currentDay);
                        bool matchPrevDay = (isPassDay && day == getPrevDay());
                        if (matchToday || matchPrevDay)
                        {
                            bool inTimeRange = false;
    
                            if (!isPassDay)
                            {
                                inTimeRange = (currentSeconds >= startSeconds && currentSeconds < endSeconds);
                            }
                            else
                            {
                                // Split over midnight: (e.g., 23:50 to 00:10)
                                inTimeRange = (currentSeconds >= startSeconds || currentSeconds < endSeconds);
                            }
    
                            if (inTimeRange)
                            {
                                if (!is_running){
                                    turn(true);
                                    break;
                                }
                            }
                            else
                            {
                                if (is_running)
                                    turn(false);
                            }
                        }
                    }
                }else{
                    bool inTimeRange = false;

                    if (!isPassDay)
                        {
                            inTimeRange = (currentSeconds >= startSeconds && currentSeconds < endSeconds);
                        }
                        else
                        {
                            // Split over midnight: (e.g., 23:50 to 00:10)
                            inTimeRange = (currentSeconds >= startSeconds || currentSeconds < endSeconds);
                        }
                        if (inTimeRange)
                        {
                            if (!is_running){
                                turn(true);
                                break;
                            }
                                
                        }
                        else
                        {
                            if (is_running){
                                schedules[i]["status"] = false;
                                turn(false);
                                updateSchedule(schedules[i]);
                            }
                                
                        }
                }
            }
        }
    }
    else if (mode == "threshold")
    {
        if(name == "wind"){
            if (value >= threshold_max && !is_running)
            {
                turn(true);
            }
            else if (value <= threshold_min && is_running)
            {
                turn(false);
            }
        }else{
            if (value <= threshold_min && !is_running)
            {
                turn(true);
            }
            else if (value >= threshold_max && is_running)
            {
                turn(false);
            }
        }

    }
}

// Update from API data
void Control::updateFromJson(const StaticJsonDocument<512> &doc)
{
    if (doc.containsKey("name") && doc.containsKey("status") &&
        doc.containsKey("threshold_min") && doc.containsKey("threshold_max") &&
        doc.containsKey("mode") && doc.containsKey("_id") && doc.containsKey("schedules"))
    {
        setStatus(doc["status"].as<bool>());
        setThresholdMin(doc["threshold_min"].as<float>());
        setThresholdMax(doc["threshold_max"].as<float>());
        setMode(doc["mode"].as<String>());
        setId(doc["_id"].as<String>());
        setSchedules(doc["schedules"]);
        setName(doc["name"]);
        if (name == "wind") {
            setPin(fanPin);
        }
        else if (name == "light") {
            setPin(lightPin);
        }
        else if (name == "water") {
            setPin(pumpPin);
        }
        else {
            Serial.println("Unknown device name: " + name);
        }
        
    }
}

