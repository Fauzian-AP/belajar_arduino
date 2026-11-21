// Definition
const int pinMerah = 25;
const int pinKuning = 26;
const int pinHijau = 27;
const int pinBuzzer = 21;

// Function
void bunyiBeep(int durasi = 150) {
  digitalWrite(pinBuzzer, HIGH);
  delay(durasi);
  digitalWrite(pinBuzzer, LOW);
}

// Setup
void setup() {
  // Memasang Pin
  pinMode(pinMerah, OUTPUT);
  pinMode(pinKuning, OUTPUT);
  pinMode(pinHijau, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);
}


void loop() {
  // Jeda awal
  digitalWrite(pinMerah, LOW);
  digitalWrite(pinKuning, LOW);
  digitalWrite(pinHijau, LOW);
  delay(1000);

  // Awal Persiapan
  digitalWrite(pinMerah, HIGH);
  digitalWrite(pinKuning, HIGH);
  digitalWrite(pinHijau, HIGH);
  bunyiBeep(200);
  delay(1000);

  // Merah Nyala
  digitalWrite(pinMerah, HIGH);
  digitalWrite(pinKuning, LOW);
  digitalWrite(pinHijau, LOW);
  bunyiBeep(200);
  delay(1000);

  // Kuning Nyala
  digitalWrite(pinMerah, LOW);
  digitalWrite(pinKuning, HIGH);
  digitalWrite(pinHijau, LOW);
  bunyiBeep(200);
  delay(1000);

  // Hijau Nyala
  digitalWrite(pinMerah, LOW);
  digitalWrite(pinKuning, LOW);
  digitalWrite(pinHijau, HIGH);
  bunyiBeep(2000);
  delay(2000);
}

