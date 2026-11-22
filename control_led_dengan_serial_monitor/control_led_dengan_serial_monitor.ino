
/*
=========================================
   KONTROL LED: ESP32 + SERIAL MONITOR
=========================================

ALUR:
- Ketik 1 pada Serial Monitor -> LED menyala.
- Ketik 0 pada Serial Monitor -> LED mati.

PEMASANGAN:
- LED : Anoda (+) -> D4 melalui resistor 220Ω
- LED : Katoda (-) -> GND
*/


// ================================================
// 1. KONFIGURASI PIN
// ================================================

const int LED_PIN = 4;


// ================================================
// 2. SETUP
// ================================================

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);

  Serial.println("Sistem siap.");
  Serial.println("Ketik 1 (nyala) atau 0 (mati).");
}


// ================================================
// 3. LOOP
// ================================================

void loop() {
  // Periksa apakah terdapat data dari Serial Monitor.
  if (Serial.available() > 0) {
    // Ambil satu karakter dari Serial Monitor.
    char command = Serial.read();

    // Abaikan karakter Enter (Newline & Carriage Return).
    if (command == '\n' || command == '\r') {
      return;
    }

    // Proses perintah yang diterima.
    switch (command) {
      case '1':
        digitalWrite(LED_PIN, HIGH);
        Serial.println("LED Menyala.");
        break;

      case '0':
        digitalWrite(LED_PIN, LOW);
        Serial.println("LED Mati.");
        break;

      default:
        Serial.println("Perintah tidak valid! Gunakan 1 atau 0.");
        break;
    }
  }
}
