const int LED_PIN = 4;

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);

  Serial.println("Siap. ketik 1 (nyala) atau 0 (mati).");
}

void loop() {
  if (Serial.available() > 0) {
    char perintah = Serial.read();

    if (perintah == '1') {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED Menyala.");
    } else if (perintah == '0') {
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED Mati.");
    }
  }
}
