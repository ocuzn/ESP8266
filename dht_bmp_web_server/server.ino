ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", webpage);
}

void handleData() {
  float dht_humidity = dht.readHumidity();
  float dht_temperature = dht.readTemperature(); // Celsius

  // Check if reads failed
  if (isnan(dht_humidity) || isnan(dht_temperature)) {
    server.send(500, "application/json", "{\"error\":\"Failed to read sensor\"}");
    return;
  }

  float bmp_temperature = bmp.readTemperature();
  float bmp_pressure = bmp.readPressure();
  // Convert pressure to hPa
  float bmp_pressure_hPa = bmp_pressure / 100.0;
  float bmp_altitude = bmp.readAltitude();


  String json = "{";
  json += "\"dht_temperature\":" + String(dht_temperature) + ",";
  json += "\"dht_humidity\":" + String(dht_humidity) + ",";

  json += "\"bmp_temperature\":" + String(bmp_temperature) + ",";
  json += "\"bmp_pressure_hPa\":" + String(bmp_pressure_hPa) + ",";
  json += "\"bmp_altitude\":" + String(bmp_altitude);

  json += "}";
  server.send(200, "application/json", json);
}