

DhtSensorData readDHTSensor() {
  DhtSensorData data;
  
  data.humidity = dht.readHumidity();
  data.temperature = dht.readTemperature(); // Celsius
  
  // Check if reads failed
  if (isnan(data.humidity) || isnan(data.temperature)) {
    data.valid = false;
    Serial.println("DHT sensor read failed");
  } else {
    data.valid = true;
  }
  
  return data;
}