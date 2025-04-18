#include "report.h"

// Constructor
Report::Report()
    : water_usage(0.0), moisture_avg(0.0), humidity_avg(0.0),
      temperature_avg(0.0), stream_avg(0.0), luminosity_avg(0.0)
{}

// Getters
float Report::getWaterUsage() const {
    return water_usage;
}

float Report::getMoistureAvg() const {
    return moisture_avg;
}

float Report::getHumidityAvg() const {
    return humidity_avg;
}

float Report::getTemperatureAvg() const {
    return temperature_avg;
}

float Report::getStreamAvg() const {
    return stream_avg;
}

float Report::getLuminosityAvg() const {
    return luminosity_avg;
}

// Setters
void Report::setWaterUsage(float newFloat) {
    water_usage = newFloat;
}

void Report::setMoistureAvg(float newFloat) {
    moisture_avg = newFloat;
}

void Report::setHumidityAvg(float newFloat) {
    humidity_avg = newFloat;
}

void Report::setTemperatureAvg(float newFloat) {
    temperature_avg = newFloat;
}

void Report::setStreamAvg(float newFloat) {
    stream_avg = newFloat;
}

void Report::setLuminosityAvg(float newFloat) {
    luminosity_avg = newFloat;
}
// Import from JSON
void Report::fromJson(StaticJsonDocument<512> doc) {
    water_usage = doc["water_usage"] | 0.0;

    moisture_avg = doc["moisture_avg"][0] | 0.0;
    humidity_avg = doc["humidity_avg"][0] | 0.0;
    temperature_avg = doc["tempurature_avg"][0] | 0.0;
    stream_avg = doc["stream_avg"][0] | 0.0;
    luminosity_avg = doc["luminosity_avg"][0] | 0.0;
}

// Export to JSON
StaticJsonDocument<512> Report::toJson() const {
    StaticJsonDocument<512> doc;
    doc["water_usage"] = water_usage;

    JsonArray moistureArr = doc.createNestedArray("moisture_avg");
    moistureArr.add(moisture_avg);

    JsonArray humidityArr = doc.createNestedArray("humidity_avg");
    humidityArr.add(humidity_avg);

    JsonArray tempArr = doc.createNestedArray("tempurature_avg");
    tempArr.add(temperature_avg);

    JsonArray streamArr = doc.createNestedArray("stream_avg");
    streamArr.add(stream_avg);

    JsonArray lumArr = doc.createNestedArray("luminosity_avg");
    lumArr.add(luminosity_avg);
    return doc;
}