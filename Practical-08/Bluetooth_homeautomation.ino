#include <SoftwareSerial.h>

// HC-05 connections
// HC-05 TXD -> Arduino D10
// HC-05 RXD -> Arduino D11 (through voltage reduction)

SoftwareSerial bluetooth(10, 11);

const int ledPin = 13;
char command;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  bluetooth.begin(9600);
  Serial.begin(9600);

  Serial.println("Home Automation System Ready");
  Serial.println("Send 1 to turn LED ON");
  Serial.println("Send 0 to turn LED OFF");
}

void loop() {
  if (bluetooth.available()) {
    command = bluetooth.read();

    if (command == '1') {
      digitalWrite(ledPin, HIGH);
      Serial.println("LED ON");
    }
    else if (command == '0') {
      digitalWrite(ledPin, LOW);
      Serial.println("LED OFF");
    }
  }
}