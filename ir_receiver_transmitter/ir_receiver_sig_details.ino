/*
 * ESP8266 IR Receiver - Capture and Display IR Signals
 * 
 * This code receives IR signals and displays the decoded information
 * via Serial Monitor. Compatible with most common IR remote protocols.
 * 
 * Hardware Requirements:
 * - ESP8266 (NodeMCU, Wemos D1 Mini, etc.)
 * - IR Receiver Module (e.g., TSOP4838, VS1838B)
 * - Optional: LED for visual feedback
 * 
 * Connections:
 * - IR Receiver VCC -> 3.3V
 * - IR Receiver GND -> GND  
 * - IR Receiver OUT -> GPIO 14 (D5 on NodeMCU)
 * - Optional LED -> GPIO 2 (D4 on NodeMCU) with 220Ω resistor
 */

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

// Pin definitions
const uint16_t IR_RECEIVE_PIN = 14;  // GPIO 14 (D5 on NodeMCU)
const uint16_t LED_PIN = 2;          // Built-in LED (GPIO 2)

// Buffer size for capturing raw IR data
const uint16_t CAPTURE_BUFFER_SIZE = 1024;

// Create IR receiver object
IRrecv irrecv(IR_RECEIVE_PIN, CAPTURE_BUFFER_SIZE);
decode_results results;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println();
  Serial.println("ESP8266 IR Receiver Starting...");
  
  // Initialize LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // Turn off LED (active low)
  
  // Enable IR receiver
  irrecv.enableIRIn();
  
  Serial.println("IR Receiver initialized and ready!");
  Serial.println("Point your remote at the IR receiver and press buttons.");
  Serial.println("----------------------------------------");
}

void loop() {
  // Check if IR signal received
  if (irrecv.decode(&results)) {
    // Flash LED to indicate signal received
    digitalWrite(LED_PIN, LOW);   // Turn on LED
    delay(100);
    digitalWrite(LED_PIN, HIGH);  // Turn off LED
    
    // Print timestamp
    Serial.print("Timestamp: ");
    Serial.println(millis());
    
    // Print protocol information
    Serial.print("Protocol: ");
    Serial.println(typeToString(results.decode_type));
    
    // Print the raw data in different formats
    Serial.print("Hex Code: 0x");
    Serial.println(uint64ToString(results.value, 16));
    
    Serial.print("Decimal: ");
    Serial.println(uint64ToString(results.value, 10));
    
    Serial.print("Binary: 0b");
    Serial.println(uint64ToString(results.value, 2));
    
    // Print bit count
    Serial.print("Bits: ");
    Serial.println(results.bits);
    
    // Print address (if available)
    if (results.decode_type == NEC || results.decode_type == SONY || 
        results.decode_type == RC5 || results.decode_type == RC6) {
      Serial.print("Address: 0x");
      Serial.println(results.address, HEX);
      Serial.print("Command: 0x");
      Serial.println(results.command, HEX);
    }
    
    // Handle repeat signals
    if (results.repeat) {
      Serial.println("REPEAT signal detected");
    }
    
    // Print raw timing data (useful for analysis)
    Serial.print("Raw Timing (");
    Serial.print(results.rawlen);
    Serial.println(" samples):");
    
    for (uint16_t i = 1; i < results.rawlen; i++) {
      if (i % 8 == 1) Serial.print("  ");
      Serial.print(results.rawbuf[i] * kRawTick);
      if (i < results.rawlen - 1) Serial.print(", ");
      if (i % 8 == 0) Serial.println();
    }
    Serial.println();
    
    // Print additional info
    Serial.print("Total samples: ");
    Serial.println(results.rawlen);
    
    Serial.println("========================================");
    
    // Resume receiving
    irrecv.resume();
  }
  
  // Small delay to prevent overwhelming the serial output
  delay(50);
}

/*
 * Optional: Function to identify unknown protocols
 * Call this in loop() if you want to analyze unknown signals
 */
void analyzeUnknownSignal() {
  if (results.decode_type == UNKNOWN) {
    Serial.println("Unknown protocol detected!");
    Serial.print("Raw length: ");
    Serial.println(results.rawlen);
    
    // Look for patterns in the raw data
    Serial.println("Analyzing timing patterns...");
    
    // Print raw timings for manual analysis
    for (uint16_t i = 1; i < results.rawlen; i++) {
      uint32_t timing = results.rawbuf[i] * kRawTick;
      Serial.print(timing);
      if (i < results.rawlen - 1) Serial.print(", ");
      if (i % 10 == 0) Serial.println();
    }
    Serial.println();
  }
}
