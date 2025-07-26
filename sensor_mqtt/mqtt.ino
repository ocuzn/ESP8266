void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(DEVICE_ID)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


// Function to publish sensor data via MQTT
void publishSensorData(const String& payload) {
  String topic = "sensors/" + String(DEVICE_ID) + "/data";
  if (client.publish(topic.c_str(), payload.c_str())) {
    Serial.println("Data published successfully to: " + topic);
  } else {
    Serial.println("Failed to publish data to: " + topic);
  }
}