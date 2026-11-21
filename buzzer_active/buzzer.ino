const int pinBuzzer = 21;

void setup() {
  pinMode(pinBuzzer, OUTPUT);
}

void loop() {
  digitalWrite(pinBuzzer,HIGH);
  delay(200);
  digitalWrite(pinBuzzer,LOW);
  delay(800);
}
