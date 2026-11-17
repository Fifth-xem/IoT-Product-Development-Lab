
#define BLYNK_TEMPLATE_ID "TMPL3PJEHHwRZ"
#define BLYNK_TEMPLATE_NAME "Smart Temperature Fan"
#define BLYNK_AUTH_TOKEN "sukeJu4QLK0eXotmw_shoRz1Uhs9mVU7"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

// Wi-Fi hotspot details
char ssid[] = "Network73";
char pass[] = "87654321";

// ESP32 pin configuration
#define DHTPIN 4
#define FANPIN 23
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// Temperature threshold
const float TEMP_THRESHOLD = 28.0;

// Active LOW relay
const int FAN_ON = LOW;
const int FAN_OFF = HIGH;

// Operating mode: 0 = AUTO, 1 = MANUAL
int operatingMode = 0;

// Manual fan command: 0 = OFF, 1 = ON
int manualFan = 0;

int fanStatus = 0;

// Apply fan control
void controlFan() {
  if (operatingMode == 0) {
    // Automatic mode
    // Temperature-based decision is made in readSensor()
    return;
  }

  // Manual mode
  if (manualFan == 1) {
    digitalWrite(FANPIN, FAN_ON);
    fanStatus = 1;
  } else {
    digitalWrite(FANPIN, FAN_OFF);
    fanStatus = 0;
  }

  Blynk.virtualWrite(V4, fanStatus);
}

// Operating Mode switch: V2
BLYNK_WRITE(V2) {
  operatingMode = param.asInt();

  Serial.print("Operating Mode: ");
  Serial.println(operatingMode == 0 ? "AUTO" : "MANUAL");

  if (operatingMode == 1) {
    controlFan();
  }
}

// Manual Fan switch: V3
BLYNK_WRITE(V3) {
  manualFan = param.asInt();

  if (operatingMode == 1) {
    controlFan();
  }
}

// Restore app switch values after connection
BLYNK_CONNECTED() {
  Blynk.syncVirtual(V2, V3);
}

// Read temperature and humidity
void readSensor() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("DHT sensor reading failed!");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.println(" %");

  // Send readings to Blynk
  Blynk.virtualWrite(V0, temp);
  Blynk.virtualWrite(V1, hum);

  // Automatic mode
  if (operatingMode == 0) {
    if (temp >= TEMP_THRESHOLD) {
      digitalWrite(FANPIN, FAN_ON);
      fanStatus = 1;
      Serial.println("Fan ON - AUTO");
    } else {
      digitalWrite(FANPIN, FAN_OFF);
      fanStatus = 0;
      Serial.println("Fan OFF - AUTO");
    }

    Blynk.virtualWrite(V4, fanStatus);
  }
}

// Try reconnecting to Blynk
void reconnectBlynk() {
  if (WiFi.status() == WL_CONNECTED && !Blynk.connected()) {
    Blynk.connect(1000);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(FANPIN, OUTPUT);
  digitalWrite(FANPIN, FAN_OFF);

  dht.begin();

  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, pass);

  Blynk.config(BLYNK_AUTH_TOKEN);

  // Read sensor every 2 seconds
  timer.setInterval(2000L, readSensor);

  // Check Blynk connection every 10 seconds
  timer.setInterval(10000L, reconnectBlynk);

  Serial.println("Temperature Fan System Started");
}

void loop() {
  Blynk.run();
  timer.run();
}
