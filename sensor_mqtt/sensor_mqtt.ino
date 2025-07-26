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

WiFiClient espClient;
PubSubClient client(espClient);

void loop() {
  ArduinoOTA.handle();  // This must run continuously
  
  if (!client.connected()) {
    reconnect();
  }
  float dht_humidity = dht.readHumidity();
  float dht_temperature = dht.readTemperature(); // Celsius

  // Check if reads failed
  if (isnan(dht_humidity) || isnan(dht_temperature)) {
    Serial.println("Error: Failed to readh dht sensor");
    return;
  }


  // Build JSON payload
  String payload = "{";
  payload += "\"device_id\":\"" + String(device_id) + "\",";
  payload += "\"dht_temperature\":" + String(dht_temperature) + ",";
  payload += "\"dht_humidity\":" + String(dht_humidity);
  payload += "}";


  // Publish to MQTT topic
  String topic = "sensors/" + String(device_id) + "/data";
  client.publish(topic.c_str(), payload.c_str());
  delay(5000); // Send every 5 seconds

}