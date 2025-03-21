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
// WiFi credentials
String serverAddress = "https://capstone-project-iot-1.onrender.com/api/"; 
String sendSensorDataPath = "sensor/create"; 
String getSensorDataPath = "sensor/getData"; 
WiFiManager wifiManager;
AsyncWebServer server(80);
String deviceID = "";
 const float delayTime = 1000;
// DHT sensor configuration
#define DHTTYPE DHT11
#define DHTPIN 4

#define LedRPin 12
#define LedGPin 27
#define LedBPin 14
// Sensor pins
int moisturePin = 36; // Soil moisture sensor
int rainPin = 39;     // Rain sensor
int lightPin = 34;    // Light sensor
int waterFlowPin = 35;    // Light sensor

// Relay pins
int pumpPin = 16;     // Pump relay
int bulbPin = 19;    // Light relay
int fanPin = 21;      // Fan relay


bool pumpState = false;
bool lightState = false;
bool fanState = false;

bool isStart = false;

int count = 0;

// Threshold values
float tempThreshold = 30.0;        // Temperature threshold in °C
float moistureThreshold = 40.0;    // Moisture threshold in %

DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor
WiFiClient espClient;     // WiFi client

volatile int pulseCount = 0; // Count pulses from water flow sensor
float flowRate = 0.0;        // Flow rate in L/min
float totalWaterUsed = 0.0;  // Total water used in liters

// Calibration factor (adjust based on your sensor's datasheet)
const float calibrationFactor = 7.5; 
unsigned long lastPulseTime = 0;
const int debounceDelay = 20; 
// Interrupt function to count pulses
void IRAM_ATTR pulseCounter() {
  unsigned long currentTime = millis();
  if (currentTime - lastPulseTime > debounceDelay) {
    pulseCount++;
    lastPulseTime = currentTime;
  }
}
void handleControlAllRequest(AsyncWebServerRequest *request) {
  if (request->hasParam("status")) {
    String status = request->getParam("status")->value();

    pumpState = status;
    lightState = status;
    fanState = status;
    if(status){
      digitalWrite(pumpPin, LOW);
      digitalWrite(bulbPin, LOW);
      digitalWrite(fanPin, LOW);
    }else{
      digitalWrite(pumpPin, HIGH);
      digitalWrite(bulbPin, HIGH);
      digitalWrite(fanPin, HIGH);
    }
    
    
    request->send(200, "application/json", "Hehe");
  } else {
    request->send(400, "application/json", "{\"message\":\"Missing parameters\"}");
  }
}
void connectWiFi() {
  WiFi.mode(WIFI_STA);  // Set ESP32 to Station mode
  if (!wifiManager.autoConnect("ESP32_Setup")) {
    Serial.println("Failed to connect. Restarting...");
    delay(1000);
    ESP.restart();
  }
  Serial.println("Connected to WiFi!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  // Ensure automatic reconnection
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  server.on("/controlAll", HTTP_POST, handleControlAllRequest);

  server.begin(); 
}
void getDataFromServer() {
  HTTPClient http;
  
  String url = serverAddress + getSensorDataPath; // Construct the full URL
  Serial.println("Requesting data from: " + url);

  http.begin(url); // Initialize HTTP request
  int httpResponseCode = http.GET(); // Send GET request

  if (httpResponseCode > 0) { // Check for a valid response
    Serial.printf("HTTP Response Code: %d\n", httpResponseCode);
    
    String response = http.getString(); // Read the server response
    Serial.println("Server Response:");
    Serial.println(response);
    
    // **Optional: Parse JSON response**
    // parseJSON(response);
    
  } else {
    Serial.printf("Error in GET request: %s\n", http.errorToString(httpResponseCode).c_str());
  }

  http.end(); // Close the connection
}
void sendDataToServer(float soilMoisture, float rainCover, float lightIntensity, float humidity, float temperature) {
  HTTPClient http;

  // Create JSON payload
  String payload = "{";
  payload += "\"idDevice\": \"" + String(deviceID) + "\",";
  payload += "\"ipDevice\": \"" + String(soilMoisture) + "\",";
  payload += "\"soilMoisture\": " + String(soilMoisture) + ",";
  payload += "\"temperature\": " + String(temperature) + ",";
  payload += "\"humidity\": " + String(humidity) + ",";
  payload += "\"rainCover\": " + String(rainCover) + ",";
  payload += "\"lightIntensity\": " + String(lightIntensity);
  payload += "}";

  // Initialize HTTP request
  Serial.println(payload);
  http.begin(serverAddress+sendSensorDataPath);
  http.addHeader("Content-Type", "application/json");

  // Send POST request
  int httpResponseCode = http.POST(payload);

  // Check response
  if (httpResponseCode > 0) {
    Serial.printf("HTTP Response Code: %d\n", httpResponseCode);
    Serial.println(http.getString()); // Print server response
  } else {
    Serial.printf("Error in sending POST: %s\n", http.errorToString(httpResponseCode).c_str());
  }

  // End HTTP connection
  http.end();
}

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
float readSoilMoisture() {
  return map(analogRead(moisturePin), 4095, 0, 0, 100); // Convert to percentage
}
float readWaterFlowSensor() {
  return map(analogRead(waterFlowPin), 4095, 0, 0, 100); // Convert to percentage
}
float readRainSensor() {
  return map(analogRead(rainPin), 4095, 0, 0, 100); // Convert to percentage
}

float readLightSensor() {
  return map(analogRead(lightPin), 4095, 0, 0, 100); // Convert to percentage
}

void controlFan(float temperature) {
  if(!fanState){
    return;
  }
  if (temperature > tempThreshold) {
    Serial.println("Temperature too high! Turning on fan...");
    digitalWrite(fanPin, LOW); // Activate fan
  } else {
    digitalWrite(fanPin, HIGH); // Deactivate fan
  }
}

void controlPump(float soilMoisture) {
  if(!pumpState){
    return;
  }
  if (soilMoisture < moistureThreshold) {
    Serial.println("Soil moisture too low! Turning on pump...");
    digitalWrite(pumpPin, LOW); // Activate pump
  } else {
    digitalWrite(pumpPin, HIGH); // Deactivate pump
  }
}

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);

  dht.begin();
  getChipID();
  // Set pin modes
  pinMode(moisturePin, INPUT);
  pinMode(rainPin, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(waterFlowPin, INPUT_PULLUP);
  pinMode(fanPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(bulbPin, OUTPUT);
  pinMode(LedBPin, OUTPUT);
  pinMode(LedGPin, OUTPUT);
  pinMode(LedRPin, OUTPUT);



  attachInterrupt(digitalPinToInterrupt(waterFlowPin), pulseCounter, FALLING);
  // Initialize relays to off
  digitalWrite(fanPin, HIGH);
  digitalWrite(pumpPin, HIGH);
  digitalWrite(bulbPin, HIGH);

  // Connect to WiFi and MQTT
  connectWiFi();
  Serial.println("ESP32 iP: "+WiFi.localIP().toString());
}
void setColor(int red, int green, int blue) {
  analogWrite(LedRPin, red);
  analogWrite(LedGPin, green);
  analogWrite(LedBPin, blue);
}
void rainbowCycle() {
  int delayTime = 10; // Adjust for speed

  for (int i = 0; i < 256; i++) { setColor(255, i, 0); delay(delayTime); } // Red to Yellow
  for (int i = 0; i < 256; i++) { setColor(255 - i, 255, 0); delay(delayTime); } // Yellow to Green
  for (int i = 0; i < 256; i++) { setColor(0, 255, i); delay(delayTime); } // Green to Cyan
  for (int i = 0; i < 256; i++) { setColor(0, 255 - i, 255); delay(delayTime); } // Cyan to Blue
  for (int i = 0; i < 256; i++) { setColor(i, 0, 255); delay(delayTime); } // Blue to Magenta
  for (int i = 0; i < 256; i++) { setColor(255, 0, 255 - i); delay(delayTime); } // Magenta to Red
}


void loop() {
  if (Serial.available()) {  // Check if data is available
    char inputChar = Serial.read();  // Read the character
    
    if(inputChar)
      isStart = !isStart;
  }
  if(!isStart)
    return;
  setColor(0,256,0);
  pulseCount = 0; // Reset pulse count
  delay(1000);    // Measure pulses for 1 second
  flowRate = pulseCount / calibrationFactor;  // Convert to L/min
  totalWaterUsed += flowRate / 60;    // Accumulate total water usage
  float soilMoisture = readSoilMoisture();
  float rainCover = readRainSensor();
  float lightIntensity = readLightSensor();
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check DHT sensor readings
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(delayTime);
    return;
  }
  
  // Print sensor data
  Serial.println("Sensor Data:");
  Serial.printf("Water Flow Rate: %.2f L/min\n", flowRate);
  Serial.printf("Total Water Used: %.2f L\n", totalWaterUsed);
  Serial.printf("Soil Moisture: %.2f %%\n", soilMoisture);
  Serial.printf("Rain Cover: %.2f %%\n", rainCover);
  Serial.printf("Light Intensity: %.2f %%\n", lightIntensity);
  Serial.printf("Humidity: %.2f %%\n", humidity);
  Serial.printf("Temperature: %.2f *C\n", temperature);

  // Control relays

  controlFan(temperature);
  controlPump(soilMoisture);
  // sendDataToServer(soilMoisture,rainCover,lightIntensity,humidity,temperature);
  // getDataFromServer();
  Serial.println("------------------------------------------------------------------");
  // delay(delayTime); // Adjust delay as needed
}
