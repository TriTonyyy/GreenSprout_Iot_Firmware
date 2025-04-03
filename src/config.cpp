#include "config.h"

String serverAddress = "https://capstone-project-iot-1.onrender.com/api/";
// String serverAddress = "http://192.168.1.214:8000/api/";
String createControlPath = "control/createControl"; 
String getControlPath = "control/detailControlBy/"; 
String updateControlPath = "control/updateControlBy/"; 

String createSensorPath = "sensor/createSensor"; 
// String createSensorPath = "sensor/createSensor"; 
String getSensorPath = "sensor/detailSensorBy/"; 
String updateSensorPath = "sensor/updateSensorBy/"; 

String createDevicePath = "device/createDevice"; 
String getDevicePath = "device/detailDeviceBy/"; 
String updateDevicePath = "device/updateDeviceBy/"; 

String getSchedulePath = "schedule/detailScheduleBy/"; 

String deviceID = "";  // Lưu ID thiết bị
String streamLabel = "stream";
String humidityLabel = "humidity";
String moistureLabel = "moisture";
String luminosityLabel = "luminosity";
String temperatureLabel = "temperature";

AsyncWebServer server(80);  // Khởi tạo server


String getDeviceID(){
    uint64_t chipId = ESP.getEfuseMac();
    char deviceID[17]; // ESP32 has a 6-byte MAC, but 64-bit integer needs up to 16 chars + null terminator
    sprintf(deviceID, "%llX", chipId); // Convert to hexadecimal string
    return String(deviceID);
}