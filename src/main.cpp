#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
// WiFi credentials
const char* ssid = "HEN COFFEE";
const char* password = "18032024";
const char* serverName = "https://capstone-project-iot-1.onrender.com/api/sensor/data"; 
// MQTT broker details
// const char* mqttServer = "test.mosquitto.org";
// const int mqttPort = 1883;
 const float delayTime = 10000;
// DHT sensor configuration
#define DHTTYPE DHT11
#define DHTPIN 4

// Sensor pins
int moisturePin = 36; // Soil moisture sensor
int rainPin = 39;     // Rain sensor
int lightPin = 34;    // Light sensor

// Relay pins
int pumpPin = 16;     // Pump relay
int motorPin = 19;    // Motor relay
int fanPin = 21;      // Fan relay

// Threshold values
float tempThreshold = 30.0;        // Temperature threshold in °C
float moistureThreshold = 40.0;    // Moisture threshold in %

DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor
WiFiClient espClient;     // WiFi client
// PubSubClient client(espClient); // MQTT client

void connectWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(WiFi.status());
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
}
void sendDataToServer(float soilMoisture, float rainCover, float lightIntensity, float humidity, float temperature) {
  HTTPClient http;

  // Create JSON payload
  String payload = "{";
  payload += "\"soilMoisture\":" + String(soilMoisture) + ",";
  payload += "\"rainCover\":" + String(rainCover) + ",";
  payload += "\"lightIntensity\":" + String(lightIntensity) + ",";
  payload += "\"humidity\":" + String(humidity) + ",";
  payload += "\"temperature\":" + String(temperature);
  payload += "}";

  // Initialize HTTP request
  http.begin(serverName);
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
// void connectMQTT() {
//   client.setServer(mqttServer, mqttPort);
//   while (!client.connected()) {
//     Serial.println("Connecting to MQTT...");
//     if (client.connect("ESP32Client")) { // Change client ID as needed
//     client.subscribe("TanSensor/dataTri");
//       Serial.println("Connected to MQTT broker!");
//     } else {
//       Serial.print("Failed with state ");
//       Serial.println(client.state());
//       delay(2000);
//     }
//   }
// }
void getChipID(){
  // Lấy địa chỉ MAC
  uint64_t chipId = ESP.getEfuseMac();
  
  // In chip ID ra màn hình
  Serial.print("ESP32 Chip ID: ");
  Serial.println((uint32_t)(chipId >> 32), HEX); // Phần trên của chipId
  Serial.println((uint32_t)chipId, HEX);        // Phần dưới của chipId

  // Tạo ID thiết bị (chỉ lấy phần dưới của chipId)
  String deviceId = String((uint32_t)chipId, HEX);
  Serial.println("Device ID: " + deviceId);
}
float readSoilMoisture() {
  return map(analogRead(moisturePin), 4095, 0, 0, 100); // Convert to percentage
}

float readRainSensor() {
  return map(analogRead(rainPin), 4095, 0, 0, 100); // Convert to percentage
}

float readLightSensor() {
  return map(analogRead(lightPin), 4095, 0, 0, 100); // Convert to percentage
}

void controlFan(float temperature) {
  if (temperature > tempThreshold) {
    Serial.println("Temperature too high! Turning on fan...");
    digitalWrite(fanPin, LOW); // Activate fan
  } else {
    digitalWrite(fanPin, HIGH); // Deactivate fan
  }
}

void controlPump(float soilMoisture) {
  if (soilMoisture < moistureThreshold) {
    Serial.println("Soil moisture too low! Turning on pump...");
    digitalWrite(pumpPin, LOW); // Activate pump
  } else {
    digitalWrite(pumpPin, HIGH); // Deactivate pump
  }
}

void setup() {
  Serial.begin(9600);
  dht.begin();
getChipID();
  // Set pin modes
  pinMode(moisturePin, INPUT);
  pinMode(rainPin, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(motorPin, OUTPUT);

  // Initialize relays to off
  digitalWrite(fanPin, HIGH);
  digitalWrite(pumpPin, HIGH);
  digitalWrite(motorPin, HIGH);

  // Connect to WiFi and MQTT
  connectWiFi();
  // connectMQTT();
}

void loop() {
  // if (!client.connected()) {
  //   connectWiFi();
  // }
  // client.loop();
  Serial.println(WiFi.status());
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
  Serial.printf("Soil Moisture: %.2f %%\n", soilMoisture);
  Serial.printf("Rain Cover: %.2f %%\n", rainCover);
  Serial.printf("Light Intensity: %.2f %%\n", lightIntensity);
  Serial.printf("Humidity: %.2f %%\n", humidity);
  Serial.printf("Temperature: %.2f *C\n", temperature);

  // Publish data to MQTT
  // String& payload = String("{") +
  //                  "\"soilMoisture\":" + soilMoisture + "," +
  //                  "\"rainCover\":" + rainCover + "," +
  //                  "\"lightIntensity\":" + lightIntensity + "," +
  //                  "\"humidity\":" + humidity + "," +
  //                  "\"temperature\":" + temperature + 
  //                  "}";
  // client.publish("TanSensor/dataTri", payload.c_str());
  // Serial.println("Data published to MQTT broker!");

  // Control relays
  controlFan(temperature);
  controlPump(soilMoisture);
  // sendDataToServer(soilMoisture,rainCover,lightIntensity,humidity,temperature);
  Serial.println("------------------------------------------------------------------");
  delay(delayTime); // Adjust delay as needed
}
