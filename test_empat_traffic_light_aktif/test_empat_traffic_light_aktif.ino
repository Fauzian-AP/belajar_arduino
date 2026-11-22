/*
=======================================================
   TEST LAMPU TRAFFIC LIGHT: ESP32 + 4 Traffic Light
=======================================================

ALUR: Semua lampu bagian Utara, Timur, Selatan, dan Barat termasuk lampu-lampu nya menyala

PEMASANGAN:
- Traffic Light : R -> D21, Y -> D22, G -> D23, GND -> GND
- Traffic Light : R -> D15, Y -> D16, G -> D17, GND -> GND
- Traffic Light : R -> D12, Y -> D13, G -> D14, GND -> GND
- Traffic Light : R -> D25, Y -> D26, G -> D27, GND -> GND
*/


// ================================================
// 1. KONFIGURASI PIN
// ================================================

// ARAH UTARA
const int NORTH_RED    = 21;
const int NORTH_YELLOW = 22;
const int NORTH_GREEN  = 23;

// ARAH TIMUR
const int EAST_RED     = 15;
const int EAST_YELLOW  = 16;
const int EAST_GREEN   = 17;

// ARAH SELATAN
const int SOUTH_RED    = 12;
const int SOUTH_YELLOW = 13;
const int SOUTH_GREEN  = 14;

// ARAH BARAT
const int WEST_RED     = 25;
const int WEST_YELLOW  = 26;
const int WEST_GREEN   = 27;


// ================================================
// 2. NYALAKAN SEMUA LAMPU TIAP ARAH
// ================================================

void turnOnAllLamp() {
  // Simpan tiap pin-pin lampu sesuai arahnya
  const int lampPins[] = {
    NORTH_RED, NORTH_YELLOW, NORTH_GREEN,
    EAST_RED,  EAST_YELLOW,  EAST_GREEN,
    SOUTH_RED, SOUTH_YELLOW, SOUTH_GREEN,
    WEST_RED,  WEST_YELLOW,  WEST_GREEN
  };

  // Tentukan jumlah pin lampu
  const int lampCount = sizeof(lampPins) / sizeof(lampPins[0]);

  // Jalankan Setup & nyalakan lampu pada tiap pin
  for (int i = 0; i < lampCount; i++) {
    pinMode(lampPins[i], OUTPUT);
    digitalWrite(lampPins[i], HIGH);
  }
}


// ================================================
// 2. SETUP
// ================================================

void setup() {
  // Semua lampu dinyalakan
  turnOnAllLamp();
}


// ================================================
// 3. LOOP
// ================================================

void loop() {}