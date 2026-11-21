/*
================================================
       TRAFFIC LIGHT 4 ARAH INDONESIA - ESP32
================================================

URUTAN: UTARA -> TIMUR -> SELATAN -> BARAT -> ULANG

DURASI:
- Hijau  : 10 detik
- Kuning :  5 detik

POLA SETIAP ARAH: Hijau -> Kuning -> Merah -> Kuning -> Hijau

TRANSISI:
- Arah aktif: Hijau -> Kuning
- Arah berikutnya: Merah -> Kuning
- Setelah 5 detik:
  arah aktif -> Merah
  arah berikutnya -> Hijau

CATATAN:
- Setiap arah hanya menyalakan satu warna.
- Tidak ada jeda semua lampu mati.
- Timer menggunakan millis().
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
// 2. KONFIGURASI WAKTU
// ================================================

const unsigned long GREEN_TIME  = 10000UL;
const unsigned long YELLOW_TIME = 5000UL;


// ================================================
// 3. STRUKTUR TRAFFIC LIGHT
// ================================================

struct TrafficLight {
  // Nama Arah
  const char* name;

  // Pin Warna
  int red;
  int yellow;
  int green;
};


// Urutan arah searah jarum jam
TrafficLight lights[] = {
  { "UTARA",   NORTH_RED, NORTH_YELLOW, NORTH_GREEN },
  { "TIMUR",   EAST_RED,  EAST_YELLOW,  EAST_GREEN  },
  { "SELATAN", SOUTH_RED, SOUTH_YELLOW, SOUTH_GREEN },
  { "BARAT",   WEST_RED,  WEST_YELLOW,  WEST_GREEN  }
};


const int TOTAL_DIRECTIONS = 4;


// ================================================
// 4. VARIABEL TIMER DAN STATUS
// ================================================

// Arah yang sedang mendapatkan giliran
int currentDirection = 0;

// Arah berikutnya
int nextDirection = 1;

// Waktu awal fase
unsigned long phaseStartTime = 0;

// Status fase
enum Phase {
  GREEN_PHASE,
  YELLOW_PHASE
};

// Fase aktif
Phase currentPhase = GREEN_PHASE;


// ================================================
// 5. FUNGSI LOG SEDERHANA
// ================================================

void printPhaseStatus() {
  Serial.print("Arah: ");
  Serial.print(lights[currentDirection].name);
  Serial.print(" | Fase: ");

  if (currentPhase == GREEN_PHASE) {
    Serial.println("HIJAU");
  } else {
    Serial.println("KUNING");
  }
}


// ================================================
// 6. FUNGSI MENGATUR SEMUA ARAH MERAH
// ================================================

void setAllRed() {
  for (int i = 0; i < TOTAL_DIRECTIONS; i++) {
    digitalWrite(lights[i].red, HIGH);
    digitalWrite(lights[i].yellow, LOW);
    digitalWrite(lights[i].green, LOW);
  }
}


// ================================================
// 7. FUNGSI INISIALISASI PIN
// ================================================

void initializePins() {
  for (int i = 0; i < TOTAL_DIRECTIONS; i++) {
    pinMode(lights[i].red, OUTPUT);
    pinMode(lights[i].yellow, OUTPUT);
    pinMode(lights[i].green, OUTPUT);
  }
}


// ================================================
// 8. MEMULAI ARAH PERTAMA
// ================================================

void startFirstDirection() {
  // Semua arah merah
  setAllRed();

  // Utara mendapatkan giliran pertama
  digitalWrite(lights[0].red, LOW);
  digitalWrite(lights[0].green, HIGH);

  currentDirection = 0;
  nextDirection = 1;

  currentPhase = GREEN_PHASE;

  phaseStartTime = millis();

  printPhaseStatus();
}


// ================================================
// 9. FASE HIJAU -> KUNING
// ================================================

void startYellowPhase() {
  // Arah aktif: HIJAU -> KUNING

  digitalWrite(lights[currentDirection].green, LOW);
  digitalWrite(lights[currentDirection].yellow, HIGH);

  // Arah berikutnya: MERAH -> KUNING

  digitalWrite(lights[nextDirection].red, LOW);
  digitalWrite(lights[nextDirection].yellow, HIGH);

  // Perbarui arah aktif
  currentPhase = YELLOW_PHASE;

  phaseStartTime = millis();

  printPhaseStatus();
}


// ================================================
// 10. FASE KUNING -> MERAH DAN HIJAU
// ================================================

void finishYellowPhase() {
  // Arah sebelumnya: KUNING -> MERAH

  digitalWrite(lights[currentDirection].yellow, LOW);
  digitalWrite(lights[currentDirection].red, HIGH);

  // Arah berikutnya: KUNING -> HIJAU

  digitalWrite(lights[nextDirection].yellow, LOW);
  digitalWrite(lights[nextDirection].green, HIGH);

  // Perbarui arah aktif
  currentDirection = nextDirection;

  // Tentukan arah berikutnya
  nextDirection = (currentDirection + 1) % TOTAL_DIRECTIONS;

  // Perbarui fase aktif
  currentPhase = GREEN_PHASE;

  phaseStartTime = millis();

  printPhaseStatus();
}


// ================================================
// 11. SETUP
// ================================================

void setup() {
  Serial.begin(115200);

  delay(500);

  initializePins();

  startFirstDirection();
}


// ================================================
// 12. LOOP UTAMA
// ================================================

void loop() {
  // Menghitung waktu yang telah berjalan
  unsigned long currentTime = millis();

  unsigned long elapsedTime = currentTime - phaseStartTime;

  // ----------------------------------------------
  // FASE HIJAU
  // ----------------------------------------------

  if (currentPhase == GREEN_PHASE) {
    if (elapsedTime >= GREEN_TIME) {
      startYellowPhase();
    }
  }

  // ----------------------------------------------
  // FASE KUNING
  // ----------------------------------------------

  else if (currentPhase == YELLOW_PHASE) {
    if (elapsedTime >= YELLOW_TIME) {
      finishYellowPhase();
    }
  }
}