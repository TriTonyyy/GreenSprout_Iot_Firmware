#include "model/control.h"
#include "control/service/server_api.h"
#include "view/display.h"
#include "control/manager/device_manager.h"
#include "control/service/time_manager.h"
void initControls()
{
    pinMode(pumpPin, OUTPUT);
    pinMode(fanPin, OUTPUT);
    pinMode(lightPin, OUTPUT);
}
Control::Control()
{
    this->id = "";
    this->name = "";
    this->pin = 0;
    this->status = false;
    this->threshold_min = 0;
    this->threshold_max = 100;
    this->mode = "manual";
    this->is_running = false;
    this->schedules;
}

Control::Control(String name, bool status, int threshold_min, int threshold_max, String mode, int pin)
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
    digitalWrite(pin, status ? LOW : HIGH); // Assuming LOW = on, HIGH = off for relay
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
    Serial.println("Status: "+String(status)+" Pin: "+String(pin));
    digitalWrite(pin, status ? LOW: HIGH); // LOW = on, HIGH = off for relay
}

void Control::setName(String newName)
{
    name = newName;
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
    is_running = isOn;
    if(isOn){
        setColor(255,255,0);
    }else{
        setColor(255,255,255);
        delay(1000);
        setColor(0,0,0);
    }
    std::vector<Control*> controls;
    controls.push_back(this);
    sendControls(controls);

}

void Control::toggle()
{
    setStatus(!status);
    is_running = status;
    Serial.println(status ? "Turning "+name+" on..." : "Turning "+name+" off...");
}


// Update method for threshold-based control
void Control::update(float value)
{
    Serial.println("Mode: "+mode+" Status:"+status+" IsRunning: "+is_running);
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
        String currentTime = getTime();  // "HH:MM:SS"
        
        int curH, curM, curS;
        sscanf(currentTime.c_str(), "%d:%d:%d", &curH, &curM, &curS);
        int currentSeconds = curH * 3600 + curM * 60 + curS;
        for (size_t i = 0; i < schedules.size(); i++)
        {
            if (schedules[i]["status"])
            {
                String startTime = convertTo24Hour(schedules[i]["startTime"]);
                int duration = schedules[i]["duration"].as<int>();

                int startH, startM, startS;
                sscanf(startTime.c_str(), "%d:%d:%d", &startH, &startM, &startS);
                int startSeconds = startH * 3600 + startM * 60 + startS;
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
                                if (!is_running)
                                    turn(true);
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
                            if (!is_running)
                                turn(true);
                        }
                        else
                        {
                            if (is_running){
                                turn(false);
                                schedules[i]["status"] = false;
                                updateSchedule(schedules[i]);
                            }
                                
                        }
                }
            }
        }
    }
    else if (mode == "threshold")
    {
        if (value < threshold_min && !is_running)
        {
            turn(true);
        }
        else if (value >= threshold_max && is_running)
        {
            turn(false);
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
        setThresholdMin(doc["threshold_min"].as<int>());
        setThresholdMax(doc["threshold_max"].as<int>());
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

