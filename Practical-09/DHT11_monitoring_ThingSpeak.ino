#include <ESP8266WiFi.h>
#include <DHT.h>
 
// -------------------- USER SETTINGS --------------------
const char* ssid     = "Network73";   // <-- replace with your WiFi SSID
const char* password = "12345678";          // <-- replace with your WiFi password
 
// ThingSpeak settings
const char* server   = "api.thingspeak.com";
const char* writeAPI = "BSNQKJSKJ0ODEQAR";  // <-- replace with your ThingSpeak WRITE API key
// Map your fields in the channel: Field1 = Temperature (°C), Field2 = Humidity (%RH)
 
// Sensor settings
#define DHTPIN   D1       // GPIO5 (NodeMCU pin label: D1)
#define DHTTYPE  DHT11
// ------------------------------------------------------
 
DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;
 
// Upload every ≥15 s. Use 20 s for safety.
const unsigned long UPLOAD_INTERVAL_MS = 20000;
unsigned long lastUpload = 0;
 
void connectWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
 
  // Wait for connection
  uint8_t tries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
    if (++tries >= 60) {   // ~30 seconds timeout
      Serial.println("\nWiFi connect timeout. Restarting...");
      ESP.restart();
    }
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
 
void setup() {
  Serial.begin(115200);
  delay(50);
  Serial.println();
  Serial.println("DHT11 + NodeMCU → ThingSpeak");
  dht.begin();
  connectWiFi();
}
 
bool postToThingSpeak(float temperatureC, float humidity) {
  // Build URL-encoded POST body. API key can go in header (X-THINGSPEAKAPIKEY)
  // or as field "api_key" in the body. We’ll send it in the header and keep
  // the body to fields only.
  String postBody = "field1=" + String(temperatureC, 1) + "&field2=" + String(humidity, 1);
 
  if (!client.connect(server, 80)) {
    Serial.println("Connection to ThingSpeak failed.");
    return false;
  }
 
  // HTTP request
  client.print(String("POST /update HTTP/1.1\r\n") +
               "Host: " + String(server) + "\r\n" +
               "Connection: close\r\n" +
               "X-THINGSPEAKAPIKEY: " + String(writeAPI) + "\r\n" +
               "Content-Type: application/x-www-form-urlencoded\r\n" +
               "Content-Length: " + String(postBody.length()) + "\r\n\r\n" +
               postBody + "\r\n");
 
  // Read response (optional but useful for debugging)
  unsigned long start = millis();
  while (client.connected() && millis() - start < 3000) {
    while (client.available()) {
      String line = client.readStringUntil('\n');
      // Uncomment to see entire response:
      // Serial.println(line);
      // ThingSpeak returns an integer line with the entry number if successful
      if (line.length() == 1 && line[0] == '\r') {
        // blank line before body
        String body = client.readString();
        Serial.print("ThingSpeak response (entry ID or 0): ");
        Serial.println(body);
        break;
      }
    }
  }
  client.stop();
  return true;
}
 
void loop() {
  // Respect ThingSpeak & DHT11 timing
  if (millis() - lastUpload < UPLOAD_INTERVAL_MS) {
    delay(50);
    return;
  }
 
  // Read sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius
 
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT11. Retrying next cycle...");
    lastUpload = millis(); // still wait until next interval
    return;
  }
 
  Serial.print("Temperature: ");
  Serial.print(t, 1);
  Serial.print(" °C, Humidity: ");
  Serial.print(h, 1);
  Serial.println(" %RH");
 
  // Upload to ThingSpeak
  if (postToThingSpeak(t, h)) {
    Serial.println("Update sent to ThingSpeak.");
  } else {
    Serial.println("Update failed.");
  }
 
  lastUpload = millis();
}