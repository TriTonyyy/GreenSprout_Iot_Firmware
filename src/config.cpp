#include "config.h"

String serverAddress = "https://capstone-project-iot-1.onrender.com/api/";
String createControlPath = "control/create"; 
String getControlPath = "control/detailControlBy/"; 
String updateControlPath = "control/updateControlBy/"; 

String createSensorPath = "sensor/create"; 
String getSensorPath = "sensor/detailSensorBy/"; 
String updateSensorPath = "sensor/updateSensorBy/"; 

String createDevicePath = "device/create"; 
String getDevicePath = "device/detailDeviceBy/"; 
String updateDevicePath = "device/updateDeviceBy/"; 

String deviceID = "";  // Lưu ID thiết bị
AsyncWebServer server(80);  // Khởi tạo server
