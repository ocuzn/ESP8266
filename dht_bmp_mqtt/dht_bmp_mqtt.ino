#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <WiFiManager.h>        

// DHT22
#define DHTPIN D7
#define DHTTYPE DHT22

// bmp085
#define seaLevelPressure_hPa 1013.25;
// connect to api to get this pressure


//Adafruit_BMP085 bmp;

DHT dht(DHTPIN, DHTTYPE);

// MQTT broker (Raspberry Pi IP)
const char* mqtt_server = "IP"; // Replace with your Pi's IP
const char* deviceID = "esp8266_01";
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

  //if (!bmp.begin()) {
  //  Serial.println("BMP180 Not Found. CHECK CIRCUIT!");
  //  return;
  //}

  //float bmp_temperature = bmp.readTemperature();
  //float bmp_pressure = bmp.readPressure();
  // Convert pressure to hPa
  //float bmp_pressure_hPa = bmp_pressure / 100.0;
  //float bmp_altitude = bmp.readAltitude();


  // Build JSON payload
  String payload = "{";
  payload += "\"device_id\":\"" + String(deviceID) + "\",";
  payload += "\"dht_temperature\":" + String(dht_temperature) + ",";
  payload += "\"dht_humidity\":" + String(dht_humidity);
  //payload += "\"bmp_temperature\":" + String(bmp_temperature) + ",";
  //payload += "\"bmp_pressure_hPa\":" + String(bmp_pressure_hPa) + ",";
  //payload += "\"bmp_altitude\":" + String(bmp_altitude);
  payload += "}";


  // Publish to MQTT topic
  String topic = "sensors/" + String(deviceID) + "/data";
  client.publish(topic.c_str(), payload.c_str());
  delay(5000); // Send every 5 seconds

}