/*
 * ESP8266 IR Transmitter - Send Pre-defined IR Signals
 * 
 * This code transmits IR signals based on the mappings from mappings.txt
 * Supports serial commands for testing individual IR signals.
 * 
 * Hardware Requirements:
 * - ESP8266 (NodeMCU, Wemos D1 Mini, etc.)
 * - IR LED (940nm recommended)
 * - NPN Transistor (2N2222 or similar)
 * - 100Ω resistor for IR LED
 * - 1kΩ resistor for transistor base
 * 
 * Connections:
 * - IR LED Anode -> Transistor Collector
 * - IR LED Cathode -> GND
 * - Transistor Emitter -> GND
 * - Transistor Base -> 1kΩ resistor -> GPIO 4 (D2 on NodeMCU)
 * - 100Ω resistor in series with IR LED for current limiting
 */

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

// Pin definitions
const uint16_t IR_SEND_PIN = 4;       // GPIO 4 (D2 on NodeMCU)
const uint16_t LED_PIN = 2;           // Built-in LED (GPIO 2)

// Create IR sender object
IRsend irsend(IR_SEND_PIN);

// IR signal definitions from mappings.txt
struct IRSignal {
  const char* name;
  uint32_t code;
  uint8_t bits;
};

// All IR signals from mappings.txt
const IRSignal signals[] = {
  {"ON", 0xFFA25D, 32},                    // On
  {"WHITE_LOW", 0xFF629D, 32},             // White hue, lowest brightness
  {"OFF", 0xFFE21D, 32},                   // OFF
  {"RED", 0xFF6897, 32},                   // RED Hue
  {"ORANGE", 0xFF30CF, 32},                // Orange Hue
  {"WHITE_MAX", 0xFF52AD, 32},             // White hue, Max brightness
  {"BRIGHTNESS_UP", 0xFFA857, 32},         // Increase brightness
  {"BRIGHTNESS_DOWN", 0xFF906F, 32}        // Decrease brightness
};

const int NUM_SIGNALS = sizeof(signals) / sizeof(signals[0]);

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println();
  Serial.println("ESP8266 IR Transmitter Starting...");
  
  // Initialize LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH); // Turn off LED (active low)
  
  // Initialize IR sender
  irsend.begin();
  
  Serial.println("IR Transmitter initialized and ready!");
  Serial.println("Available commands:");
  Serial.println("1 - ON");
  Serial.println("2 - WHITE_LOW (White hue, lowest brightness)");
  Serial.println("3 - OFF");
  Serial.println("4 - RED (Red Hue)");
  Serial.println("5 - ORANGE (Orange Hue)");
  Serial.println("6 - WHITE_MAX (White hue, Max brightness)");
  Serial.println("7 - BRIGHTNESS_UP (Increase brightness)");
  Serial.println("8 - BRIGHTNESS_DOWN (Decrease brightness)");
  Serial.println("9 - Send all commands in sequence");
  Serial.println("Enter command number (1-9):");
  Serial.println("----------------------------------------");
}

void loop() {
  // Check for serial input
  if (Serial.available()) {
    int command = Serial.parseInt();
    
    if (command >= 1 && command <= NUM_SIGNALS) {
      sendIRSignal(command - 1);
    } else if (command == 9) {
      sendAllSignals();
    } else {
      Serial.println("Invalid command. Enter 1-9.");
    }
    
    // Clear serial buffer
    while (Serial.available()) {
      Serial.read();
    }
  }
}

// Function to send a specific IR signal
void sendIRSignal(int signalIndex) {
  if (signalIndex < 0 || signalIndex >= NUM_SIGNALS) {
    Serial.println("Invalid signal index");
    return;
  }
  
  // Flash LED to indicate transmission
  digitalWrite(LED_PIN, LOW);   // Turn on LED
  
  // Send the NEC signal
  irsend.sendNEC(signals[signalIndex].code, signals[signalIndex].bits);
  
  Serial.print("Sent signal: ");
  Serial.print(signals[signalIndex].name);
  Serial.print(" (0x");
  Serial.print(signals[signalIndex].code, HEX);
  Serial.println(")");
  
  delay(100);
  digitalWrite(LED_PIN, HIGH);  // Turn off LED
  
  // Small delay to prevent rapid fire
  delay(200);
}

// Function to send all signals in sequence
void sendAllSignals() {
  Serial.println("Sending all signals in sequence...");
  
  for (int i = 0; i < NUM_SIGNALS; i++) {
    Serial.print("Sending ");
    Serial.print(i + 1);
    Serial.print("/");
    Serial.print(NUM_SIGNALS);
    Serial.print(": ");
    sendIRSignal(i);
    delay(1000); // Wait 1 second between signals
  }
  
  Serial.println("All signals sent!");
}

// Individual function wrappers for each command
void turnOn() {
  sendIRSignal(0);
}

void whiteLowBrightness() {
  sendIRSignal(1);
}

void turnOff() {
  sendIRSignal(2);
}

void setRedHue() {
  sendIRSignal(3);
}

void setOrangeHue() {
  sendIRSignal(4);
}

void whiteMaxBrightness() {
  sendIRSignal(5);
}

void increaseBrightness() {
  sendIRSignal(6);
}

void decreaseBrightness() {
  sendIRSignal(7);
}

// Demo function - cycles through common operations
void demoSequence() {
  Serial.println("Starting demo sequence...");
  
  turnOn();
  delay(1000);
  
  setRedHue();
  delay(1000);
  
  increaseBrightness();
  delay(500);
  increaseBrightness();
  delay(500);
  increaseBrightness();
  delay(1000);
  
  setOrangeHue();
  delay(1000);
  
  whiteMaxBrightness();
  delay(1000);
  
  decreaseBrightness();
  delay(500);
  decreaseBrightness();
  delay(500);
  
  whiteLowBrightness();
  delay(1000);
  
  turnOff();
  
  Serial.println("Demo sequence completed!");
}