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
        report->setWaterUsage(preferences.getFloat(waterUsageLabel.c_str()));
        report->setHumidityAvg(avgHumidity);
        report->setLuminosityAvg(avgLuminosity);
        report->setMoistureAvg(avgMoisture);
        report->setStreamAvg(avgStream);
        report->setTemperatureAvg(avgTemperature);
        sendReport(report);
        resetSensorRpsData();
    }
}