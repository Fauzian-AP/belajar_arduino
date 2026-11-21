int merah = 25;
int kuning = 26;
int hijau = 27;

void setup() {
  pinMode(merah, OUTPUT);
  pinMode(kuning, OUTPUT);
  pinMode(hijau, OUTPUT);
}

void loop() {
  digitalWrite(merah, HIGH);
  delay(3000);

  digitalWrite(merah, LOW);
  delay(100);

  digitalWrite(kuning, HIGH);
  delay(3000);

  digitalWrite(kuning, LOW);
  delay(100);

  digitalWrite(hijau, HIGH);
  delay(3000);

  digitalWrite(hijau, LOW);
  delay(1500);

  digitalWrite(merah, HIGH);
  digitalWrite(kuning, HIGH);
  digitalWrite(hijau, HIGH);
  delay(3000);

  digitalWrite(merah, LOW);
  digitalWrite(kuning, LOW);
  digitalWrite(hijau, LOW);
  delay(3000);
}