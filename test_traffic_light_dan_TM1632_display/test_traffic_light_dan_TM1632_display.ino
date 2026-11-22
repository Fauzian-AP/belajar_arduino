/*
========================================
=== TRAFFIC LIGHT + TM1637 COUNTDOWN ===
========================================

Durasi:
- Merah  = 10 detik
- Kuning =  3 detik
- Hija = 20 detik
- Kuning =  3 detik

TM1637:
- Menampilkan countdown sesuai warna
*/

#include <TM1637Display.h>   // Install di Library Manager


// ========================================
// PIN TRAFFIC LIGHT
// ========================================

const int LED_RED    = 25;
const int LED_YELLOW = 26;
const int LED_GREEN  = 27;


// ========================================
// PIN TM1637
// ========================================

const int TM1637_CLK = 22;
const int TM1637_DIO = 23;


// ========================================
// DURASI
// ========================================

const int RED_TIME    = 10;
const int YELLOW_TIME = 3;
const int GREEN_TIME  = 20;


// ========================================
// TM1637
// ========================================

TM1637Display display(TM1637_CLK, TM1637_DIO);


// ========================================
// FUNGSI LAMPU
// ========================================

void setTrafficLight(
  bool redOn,
  bool yellowOn,
  bool greenOn
) {
  digitalWrite(LED_RED, redOn ? HIGH : LOW);
  digitalWrite(LED_YELLOW, yellowOn ? HIGH : LOW);
  digitalWrite(LED_GREEN, greenOn ? HIGH : LOW);
}


// ========================================
// FUNGSI COUNTDOWN
// ========================================

void countdown(int duration) {

  for (int remaining = duration; remaining > 0; remaining--) {

    display.showNumberDec(remaining, false);

    Serial.print("Countdown: ");
    Serial.println(remaining);

    delay(1000);
  }
}


// ========================================
// SETUP
// ========================================

void setup() {

  Serial.begin(115200);

  // LED traffic light
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  // Pastikan semua LED mati
  setTrafficLight(false, false, false);

  // TM1637
  display.setBrightness(7);
  display.clear();

  Serial.println("================================");
  Serial.println("TRAFFIC LIGHT + TM1637");
  Serial.println("================================");
}


// ========================================
// LOOP
// ========================================

void loop() {

  // ======================================
  // MERAH - 10 DETIK
  // ======================================

  Serial.println("MERAH");

  setTrafficLight(true, false, false);

  countdown(RED_TIME);


  // ======================================
  // KUNING - 3 DETIK
  // ======================================

  Serial.println("KUNING");

  setTrafficLight(false, true, false);

  countdown(YELLOW_TIME);


  // ======================================
  // HIJAU - 20 DETIK
  // ======================================

  Serial.println("HIJAU");

  setTrafficLight(false, false, true);

  countdown(GREEN_TIME);


  // ======================================
  // KUNING - 3 DETIK
  // ======================================

  Serial.println("KUNING");

  setTrafficLight(false, true, false);

  countdown(YELLOW_TIME);


  // Bersihkan display sebelum diulang
  display.clear();
}