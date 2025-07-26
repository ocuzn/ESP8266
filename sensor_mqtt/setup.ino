void setup_ota() {
  // OTA event handlers
  ArduinoOTA.onStart([]() {
    Serial.println("Start updating...");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nUpdate finished!");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  // Start OTA
  ArduinoOTA.begin();
  Serial.println("OTA ready, waiting for upload...");
}


void setup() {
  Serial.begin(115200);
  Serial.println("Booting...");
  WiFi.begin(ssid, password);

  // Set OTA parameters (optional)
  ArduinoOTA.setHostname("ESP8266-OTA");
  // ArduinoOTA.setPassword("yourpassword"); // (optional)

  setup_ota();
  dht.begin();
  client.setServer(mqtt_server, mqtt_port);

}