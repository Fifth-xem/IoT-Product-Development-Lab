#include "DHT.h"

// ESP32 Pin configuration matching your ESP8266 setup
#define DHTPIN 4     // Connect to physical pin 2 (G2) on ESP32
#define FANPIN 23    // Connect to physical pin 5 (G14) on ESP32

#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const float TEMP_THRESHOLD = 26.0; // Celsius threshold

// Active LOW relay definitions
const int FAN_ON = LOW;
const int FAN_OFF = HIGH;

void setup() {
  // ESP32 uses 115200 baud rate perfectly for the Serial Monitor
  Serial.begin(115200);
  
  // Initialize the fan pin as output and turn it off immediately
  digitalWrite(FANPIN, FAN_OFF); 
  pinMode(FANPIN, OUTPUT);
  
  
  // Start the DHT11 sensor
  dht.begin();
  Serial.println(F("ESP32 Temperature Control System Started."));
}

void loop() {
  // Wait 2 seconds between measurements (DHT11 limitation)
  delay(2000); 

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  // Check if the sensor read failed
  if (isnan(temp) || isnan(hum)) {
    Serial.println(F("Error: Failed to read from DHT sensor!"));
    return;
  }

  // Print data to the Serial Monitor
  Serial.print(F("Temperature: "));
  Serial.print(temp);
  Serial.println(F(" °C"));
  
  Serial.print(F("Humidity: "));
  Serial.print(hum);
  Serial.println(F(" %"));

  // Temperature logic control
  if (temp >= TEMP_THRESHOLD) {
    digitalWrite(FANPIN, FAN_ON);
    Serial.println(F("Fan ON 🔥"));
  } else {
    digitalWrite(FANPIN, FAN_OFF);
    Serial.println(F("Fan OFF ❄️"));
  }
}
