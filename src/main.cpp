#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#define WM_NO_WEB_SERVER
#include <WiFiManager.h>
#include <AsyncTCP.h>
#define WEBSERVER_H
#include <ESPAsyncWebServer.h>


String serverAddress = "https://capstone-project-iot-1.onrender.com/api/sensor/"; 
String sendDataPath = "data"; 
String getDataPath = "getData"; 
String deviceID = "";
 const float delayTime = 1000;

// WiFi and MQTT Configuration
const char* mqtt_server = "3ba65b4669e74824870ec11caa6ab9cb.s1.eu.hivemq.cloud:8883";
const char* mqtt_username = "iotMember";
const char* mqtt_password = "Iot123456";

WiFiClient espClient;
PubSubClient client(espClient);
WiFiManager wifiManager;
AsyncWebServer server(80);

#define DHTTYPE DHT11
#define DHTPIN 4
DHT dht(DHTPIN, DHTTYPE);

int moisturePin = 36;
int rainPin = 39;
int lightPin = 34;
int waterFlowPin = 35;

int pumpPin = 16;
int bulbPin = 19;
int fanPin = 21;

bool pumpState = false;
bool lightState = false;
bool fanState = false;

bool isStart = false;

// Threshold values
float tempThreshold = 30.0;        // Temperature threshold in °C
float moistureThreshold = 40.0;    // Moisture threshold in %
const float calibrationFactor = 7.5; 
unsigned long lastPulseTime = 0;
const int debounceDelay = 20; 

volatile int pulseCount = 0;
float flowRate = 0.0;
float totalWaterUsed = 0.0;

void IRAM_ATTR pulseCounter() {
  unsigned long currentTime = millis();
  if (currentTime - lastPulseTime > debounceDelay) {
    pulseCount++;
    lastPulseTime = currentTime;
  }
}
// void handleControlAllRequest(AsyncWebServerRequest *request) {
//   if (request->hasParam("status")) {
//     String status = request->getParam("status")->value();

//     pumpState = status;
//     lightState = status;
//     fanState = status;
//     if(status){
//       digitalWrite(pumpPin, LOW);
//       digitalWrite(bulbPin, LOW);
//       digitalWrite(fanPin, LOW);
//     }else{
//       digitalWrite(pumpPin, HIGH);
//       digitalWrite(bulbPin, HIGH);
//       digitalWrite(fanPin, HIGH);
//     }
    
    
//     request->send(200, "application/json", "Hehe");
//   } else {
//     request->send(400, "application/json", "{\"message\":\"Missing parameters\"}");
//   }
// }
void connectWiFi() {
  WiFi.mode(WIFI_STA);
  if (!wifiManager.autoConnect("ESP32_Setup")) {
    ESP.restart();
  }
  Serial.println("Connected to WiFi!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  // Ensure automatic reconnection
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  // server.on("/controlAll", HTTP_POST, handleControlAllRequest);

  // server.begin(); 
}
// void getDataFromServer() {
//   HTTPClient http;
  
//   String url = serverAddress + getDataPath; // Construct the full URL
//   Serial.println("Requesting data from: " + url);

//   http.begin(url); // Initialize HTTP request
//   int httpResponseCode = http.GET(); // Send GET request

    Serial.printf("HTTP Response Code: %d\n", httpResponseCode);
//   if (httpResponseCode > 0) { // Check for a valid response
//     Serial.printf("HTTP Response Code: %d\n", httpResponseCode);
    
//     String response = http.getString(); // Read the server response
//     Serial.println("Server Response:");
//     Serial.println(response);
    
//     // **Optional: Parse JSON response**
//     // parseJSON(response);
    
//   } else {
//     Serial.printf("Error in GET request: %s\n", http.errorToString(httpResponseCode).c_str());
//   }}
// void sendDataToServer(float soilMoisture, float rainCover, float lightIntensity, float humidity, float temperature) {
//   HTTPClient http;

//   // Create JSON payloadString payload = "{";
//   payload += "\"soilMoisture\":" + String(soilMoisture) + ",";
//   payload += "\"rainCover\":" + String(rainCover) + ",";
//   payload += "\"lightIntensity\":" + String(lightIntensity) + ",";
//   payload += "\"humidity\":" + String(humidity) + ",";
//   payload += "\"temperature\":" + String(temperature);
//   payload += "}";

//   // Initialize HTTP request
//   http.begin(serverAddress+sendDataPath);
//   http.addHeader("Content-Type", "application/json");

//   // Send POST request
//   int httpResponseCode = http.POST(payload);

//   // Check response
//   if (httpResponseCode > 0) {
//     Serial.printf("HTTP Response Code: %d\n", httpResponseCode);
//     Serial.println(http.getString()); // Print server response
//   } else {
//     Serial.printf("Error in sending POST: %s\n", http.errorToString(httpResponseCode).c_str());
//   }

//   // End HTTP connection
//   http.end();
// }

void getChipID(){
  // Lấy địa chỉ MAC
  uint64_t chipId = ESP.getEfuseMac();
  
  // In chip ID ra màn hình
  Serial.print("ESP32 Chip ID: ");
  Serial.println((uint32_t)(chipId >> 32), HEX); // Phần trên của chipId
  Serial.println((uint32_t)chipId, HEX);        // Phần dưới của chipId

  // Tạo ID thiết bị (chỉ lấy phần dưới của chipId)
  deviceID = String((uint32_t)chipId, HEX);
  Serial.println("Device ID: " + deviceID);
}
// float readSoilMoisture() {
//   return map(analogRead(moisturePin), 4095, 0, 0, 100); // Convert to percentage
// }
// float readWaterFlowSensor() {
//   return map(analogRead(waterFlowPin), 4095, 0, 0, 100); // Convert to percentage
// }
// float readRainSensor() {
//   return map(analogRead(rainPin), 4095, 0, 0, 100); // Convert to percentage
// }

// float readLightSensor() {
//   return map(analogRead(lightPin), 4095, 0, 0, 100); // Convert to percentage
// }

// void controlFan(float temperature) {
//   if(!fanState){
//     return;
//   }
//   if (temperature > tempThreshold) {
//     Serial.println("Temperature too high! Turning on fan...");
//     digitalWrite(fanPin, LOW); // Activate fan
//   } else {
//     digitalWrite(fanPin, HIGH); // Deactivate fan
//   }
// }

// void controlPump(float soilMoisture) {
//   if(!pumpState){
//     return;
//   }
//   if (soilMoisture < moistureThreshold) {
//     Serial.println("Soil moisture too low! Turning on pump...");
//     digitalWrite(pumpPin, LOW); // Activate pump
//   } else {
//     digitalWrite(pumpPin, HIGH); // Deactivate pump
//   }
// }
void reconnectMQTT() {
  while (!client.connected()) {
    if (client.connect(deviceID, mqtt_username, mqtt_password)) {
      client.subscribe("control/fan");
      client.subscribe("control/pump");
      client.subscribe("control/light");
    } else {
      delay(5000);
    }
  }
}
void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (strcmp(topic, "control/fan") == 0) {
    fanState = message.equals("ON");
    digitalWrite(fanPin, fanState ? LOW : HIGH);
  }
  if (strcmp(topic, "control/pump") == 0) {
    pumpState = message.equals("ON");
    digitalWrite(pumpPin, pumpState ? LOW : HIGH);
  }
  if (strcmp(topic, "control/light") == 0) {
    lightState = message.equals("ON");
    digitalWrite(bulbPin, lightState ? LOW : HIGH);
  }
}

void publishData() {
  float soilMoisture = map(analogRead(moisturePin), 4095, 0, 0, 100);
  float rainCover = map(analogRead(rainPin), 4095, 0, 0, 100);
  float lightIntensity = map(analogRead(lightPin), 4095, 0, 0, 100);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  String payload = "{";
  payload += "\"soilMoisture\":" + String(soilMoisture) + ",";
  payload += "\"rainCover\":" + String(rainCover) + ",";
  payload += "\"lightIntensity\":" + String(lightIntensity) + ",";
  payload += "\"humidity\":" + String(humidity) + ",";
  payload += "\"temperature\":" + String(temperature);
  payload += "}";

  client.publish("sensor/data", payload.c_str());
}

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  dht.begin();

  pinMode(moisturePin, INPUT);
  pinMode(rainPin, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(waterFlowPin, INPUT_PULLUP);
  pinMode(fanPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(bulbPin, OUTPUT);

  digitalWrite(fanPin, HIGH);
  digitalWrite(pumpPin, HIGH);
  digitalWrite(bulbPin, HIGH);

  attachInterrupt(digitalPinToInterrupt(waterFlowPin), pulseCounter, FALLING);
  getChipID();
  connectWiFi();
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();
  // if (Serial.available()) {  // Check if data is available
  //   char inputChar = Serial.read();  // Read the character
  //   if(inputChar)
  //     isStart = !isStart;
  // }
  // if(!isStart)
  //   return;
  // pulseCount = 0; // Reset pulse count
  // delay(1000);    // Measure pulses for 1 second
  // flowRate = pulseCount / calibrationFactor;  // Convert to L/min
  // totalWaterUsed += flowRate / 60;    // Accumulate total water usage
  // float soilMoisture = readSoilMoisture();
  // float rainCover = readRainSensor();
  // float lightIntensity = readLightSensor();
  // float humidity = dht.readHumidity();
  // float temperature = dht.readTemperature();

  // // Check DHT sensor readings
  // if (isnan(humidity) || isnan(temperature)) {
  //   Serial.println("Failed to read from DHT sensor!");
  //   delay(delayTime);
  //   return;}
  //    // Print sensor data
  // Serial.println("Sensor Data:");
  // Serial.printf("Water Flow Rate: %.2f L/min\n", flowRate);
  // Serial.printf("Total Water Used: %.2f L\n", totalWaterUsed);
  // Serial.printf("Soil Moisture: %.2f %%\n", soilMoisture);
  // Serial.printf("Rain Cover: %.2f %%\n", rainCover);
  // Serial.printf("Light Intensity: %.2f %%\n", lightIntensity);
  // Serial.printf("Humidity: %.2f %%\n", humidity);
  // Serial.printf("Temperature: %.2f *C\n", temperature);

  // // Control relays

  // controlFan(temperature);
  // controlPump(soilMoisture);
  // // sendDataToServer(soilMoisture,rainCover,lightIntensity,humidity,temperature);
  // getDataFromServer();
  // Serial.println("------------------------------------------------------------------");
  // // delay(delayTime); // Adjust delay as needed
  publishData();
  delay(5000);
}
