#include "ArduinoJson.h"
#ifndef REPORT_H
#define REPORT_H
class Report
{
private:
    float water_usage;
    float moisture_avg;
    float humidity_avg;
    float temperature_avg;
    float stream_avg;
    float luminosity_avg;
public:
    Report();
    
    float getWaterUsage() const;
    float getMoistureAvg() const;
    float getHumidityAvg() const;
    float getTemperatureAvg() const;
    float getStreamAvg() const;
    float getLuminosityAvg() const;
    StaticJsonDocument<512> toJson() const;

    void setWaterUsage(float newFloat);
    void setMoistureAvg(float newFloat);
    void setHumidityAvg(float newFloat);
    void setTemperatureAvg(float newFloat);
    void setStreamAvg(float newFloat);
    void setLuminosityAvg(float newFloat);
    void fromJson(StaticJsonDocument<512> doc);
};

#endif
