#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <WiFiManager.h> 
#include "config.h"    

// DHT22
#define DHTPIN D7
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

struct DhtSensorData {
  float temperature;
  float humidity;
  bool valid;
};

WiFiClient espClient;
PubSubClient client(espClient);

String createSensorJSON(const DhtSensorData& data) {
  String payload = "{";
  payload += "\"device_id\":\"" + String(DEVICE_ID) + "\",";
  payload += "\"dht_temperature\":" + String(data.temperature, 2) + ",";
  payload += "\"dht_humidity\":" + String(data.humidity, 2);
  payload += "}";
  
  Serial.println("JSON Payload: " + payload);
  return payload;
}


void loop() {
  ArduinoOTA.handle();  // This must run continuously
  
  if (!client.connected()) {
    reconnect();
  }
  DhtSensorData sensorData = readDHTSensor();
  
  if (sensorData.valid) {
    // Create JSON payload
    String payload = createSensorJSON(sensorData);
    
    // Publish to MQTT
    publishSensorData(payload);
  } else {
    Serial.println("Error: Failed to read DHT sensor");
  }
  
  delay(5000); // Send every 5 seconds

}