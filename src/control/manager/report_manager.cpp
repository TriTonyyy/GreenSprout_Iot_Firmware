#include "report_manager.h"
#include "control/service/time_manager.h"
#include "control/service/server_api.h"
#include "control/manager/pref_manager.h"
#include "control/manager/sensor_manager.h"
#include "model/report.h"
#include "model/sensor.h"

int lastHour = -1;
void updateReportToServer(){
    int currentHour = getHour();
    if(currentHour % 2 == 0 && currentHour != lastHour){
        float avgMoisture = calAvg((moistureLabel+sub).c_str());
        float avgStream = calAvg((streamLabel+sub).c_str());
        float avgLuminosity = calAvg((luminosityLabel+sub).c_str());
        float avgTemperature = calAvg((temperatureLabel+sub).c_str());
        float avgHumidity = calAvg((humidityLabel+sub).c_str());
        Report* report = new Report();
        report->setWaterUsage(preferences.getFloat(preferences.getFloat(waterUsageLabel.c_str())));
        report->setHumidityAvg(preferences.getFloat(avgHumidity));
        report->setLuminosityAvg(preferences.getFloat(avgLuminosity));
        report->setMoistureAvg(preferences.getFloat(avgMoisture));
        report->setStreamAvg(preferences.getFloat(avgStream));
        report->setTemperatureAvg(preferences.getFloat(avgTemperature));
        sendReport(report);
        resetSensorRpsData();
    }
}