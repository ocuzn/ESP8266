#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>
#include <ESP8266WebServer.h>

// DHT22
#define DHTPIN D7
#define DHTTYPE DHT22

// bmp085
#define seaLevelPressure_hPa 1013.25
// connect to api to get this pressure
Adafruit_BMP085 bmp;

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "ssid";
const char* password = "password";

void loop() {
  ArduinoOTA.handle();  // This must run continuously
  server.handleClient();
}