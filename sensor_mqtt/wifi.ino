// Connect to WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  
  WiFiManager wifiManager;

  // Uncomment to erase saved Wi-Fi credentials (for debugging/testing)
  // wifiManager.resetSettings();

  // Auto connect will try saved credentials, or start AP if none/fail
  // "ESP_Config" is the AP SSID (name of captive portal)
  if (!wifiManager.autoConnect("ESP_Config", "secreto iberico")) {
    Serial.println("Failed to connect and timeout reached");
    ESP.restart();
    delay(1000);
  }

  Serial.println("Connected to WiFi!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("\nWiFi connected");
}