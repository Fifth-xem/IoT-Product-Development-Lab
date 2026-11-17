
const int ldrPin = A0;
const int relayPin = 7;

int lightValue = 0;
int threshold = 400;

void setup() {
  pinMode(relayPin, OUTPUT);

  Serial.begin(9600);

  digitalWrite(relayPin, HIGH);
}

void loop() {

  lightValue = analogRead(ldrPin);

  Serial.print("Light Intensity: ");
  Serial.println(lightValue);

  if (lightValue < threshold) {
    digitalWrite(relayPin, LOW);
    Serial.println("Darkness Detected - Relay ON");
  }
  else {
    digitalWrite(relayPin, HIGH);
    Serial.println("Bright Light - Relay OFF");
  }

  delay(500);
}
