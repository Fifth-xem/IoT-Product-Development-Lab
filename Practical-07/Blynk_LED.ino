#define BLYNK_TEMPLATE_ID "TMPL3Uw_W-cN2"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "hidden token i removed from code"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "Network73";
char pass[] = "12345678";

#define LED_PIN 5   // D1 = GPIO5

BLYNK_WRITE(V0)
{
  int value = param.asInt();

  if (value == 1)
  {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED ON");
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED OFF");
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.println();
  Serial.println("ESP8266 Blynk LED Control");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  Serial.println("Blynk Connected!");
  Serial.println("System Ready!");
}

void loop()
{
  Blynk.run();
}