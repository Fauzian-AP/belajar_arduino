/*
================================================
       TEST SEMUA LAMPU - ESP32
================================================

Semua lampu:
- UTARA  : Merah, Kuning, Hijau
- TIMUR  : Merah, Kuning, Hijau
- SELATAN: Merah, Kuning, Hijau
- BARAT  : Merah, Kuning, Hijau

Semua menyala terus menerus.
Tidak ada timer.
Tidak ada pergantian fase.
*/


// ================================================
// 1. KONFIGURASI PIN
// ================================================

// UTARA
const int NORTH_RED    = 21;
const int NORTH_YELLOW = 22;
const int NORTH_GREEN  = 23;

// TIMUR
const int EAST_RED     = 15;
const int EAST_YELLOW  = 16;
const int EAST_GREEN   = 17;

// SELATAN
const int SOUTH_RED    = 13;
const int SOUTH_YELLOW = 12;
const int SOUTH_GREEN  = 14;

// BARAT
const int WEST_RED     = 27;
const int WEST_YELLOW  = 26;
const int WEST_GREEN   = 25;


// ================================================
// 2. SETUP
// ================================================

void setup() {
  // Semua pin sebagai OUTPUT

  pinMode(NORTH_RED, OUTPUT);
  pinMode(NORTH_YELLOW, OUTPUT);
  pinMode(NORTH_GREEN, OUTPUT);

  pinMode(EAST_RED, OUTPUT);
  pinMode(EAST_YELLOW, OUTPUT);
  pinMode(EAST_GREEN, OUTPUT);

  pinMode(SOUTH_RED, OUTPUT);
  pinMode(SOUTH_YELLOW, OUTPUT);
  pinMode(SOUTH_GREEN, OUTPUT);

  pinMode(WEST_RED, OUTPUT);
  pinMode(WEST_YELLOW, OUTPUT);
  pinMode(WEST_GREEN, OUTPUT);

  // Semua lampu dinyalakan

  digitalWrite(NORTH_RED, HIGH);
  digitalWrite(NORTH_YELLOW, HIGH);
  digitalWrite(NORTH_GREEN, HIGH);

  digitalWrite(EAST_RED, HIGH);
  digitalWrite(EAST_YELLOW, HIGH);
  digitalWrite(EAST_GREEN, HIGH);

  digitalWrite(SOUTH_RED, HIGH);
  digitalWrite(SOUTH_YELLOW, HIGH);
  digitalWrite(SOUTH_GREEN, HIGH);

  digitalWrite(WEST_RED, HIGH);
  digitalWrite(WEST_YELLOW, HIGH);
  digitalWrite(WEST_GREEN, HIGH);
}


// ================================================
// 3. LOOP
// ================================================

void loop() {}