#include <TM1637Display.h>


/*
================================================
       TRAFFIC LIGHT 4 ARAH + 4 DISPLAY
              ESP32 + TM1637
================================================

URUTAN: UTARA -> TIMUR -> SELATAN -> BARAT

DURASI:
Hijau  : 10 detik
Kuning : 5 detik
Merah  : 40 detik

TOTAL SIKLUS: 60 detik

DISPLAY: Menampilkan countdown fase masing-masing arah.
*/


// =================================================
// 1. PIN TRAFFIC LIGHT
// =================================================

// UTARA
const int NORTH_RED    = 21;
const int NORTH_YELLOW = 22;
const int NORTH_GREEN  = 23;

// TIMUR
const int EAST_RED     = 15;
const int EAST_YELLOW  = 16;
const int EAST_GREEN   = 17;

// SELATAN
const int SOUTH_RED    = 12;
const int SOUTH_YELLOW = 13;
const int SOUTH_GREEN  = 14;

// BARAT
const int WEST_RED     = 25;
const int WEST_YELLOW  = 26;
const int WEST_GREEN   = 27;


// =================================================
// 2. PIN TM1637
// =================================================

// UTARA
const int NORTH_CLK = 18;
const int NORTH_DIO = 19;

// TIMUR
const int EAST_CLK = 4;
const int EAST_DIO = 5;

// SELATAN
const int SOUTH_CLK = 0;
const int SOUTH_DIO = 2;

// BARAT
const int WEST_CLK = 32;
const int WEST_DIO = 33;


// =================================================
// 3. OBJEK DISPLAY
// =================================================

TM1637Display displayNorth(NORTH_CLK, NORTH_DIO);
TM1637Display displayEast(EAST_CLK, EAST_DIO);
TM1637Display displaySouth(SOUTH_CLK, SOUTH_DIO);
TM1637Display displayWest(WEST_CLK, WEST_DIO);


// =================================================
// 4. KONFIGURASI WAKTU
// =================================================

const unsigned long GREEN_TIME  = 10000UL;
const unsigned long YELLOW_TIME = 5000UL;
const unsigned long RED_TIME    = 40000UL;

const unsigned long CYCLE_TIME = GREEN_TIME + YELLOW_TIME + RED_TIME + YELLOW_TIME;

// Offset hijau setiap arah
const unsigned long GREEN_OFFSET[] = {
  0UL,       // UTARA
  15000UL,   // TIMUR
  30000UL,   // SELATAN
  45000UL    // BARAT
};


// =================================================
// 5. STRUKTUR TRAFFIC LIGHT
// =================================================

struct TrafficLight {
  const char* name;

  int red;
  int yellow;
  int green;

  TM1637Display* display;
};


TrafficLight lights[] = {
  { "UTARA", NORTH_RED, NORTH_YELLOW, NORTH_GREEN, &displayNorth },
  { "TIMUR", EAST_RED, EAST_YELLOW, EAST_GREEN, &displayEast },
  { "SELATAN", SOUTH_RED, SOUTH_YELLOW, SOUTH_GREEN, &displaySouth },
  { "BARAT", WEST_RED, WEST_YELLOW, WEST_GREEN, &displayWest }
};


const int TOTAL_DIRECTIONS = 4;


// =================================================
// 6. STATUS FASE
// =================================================

enum LightPhase {
  RED_PHASE,
  YELLOW_PHASE,
  GREEN_PHASE
};


struct PhaseInfo {
  LightPhase phase;
  unsigned long remainingMs;
};


// =================================================
// 7. VARIABEL
// =================================================

int lastPhase[TOTAL_DIRECTIONS] = { -1, -1, -1, -1 };


int lastDisplayValue[TOTAL_DIRECTIONS] = { -1, -1, -1, -1 };


unsigned long cycleStartTime = 0;


// =================================================
// 8. MENGHITUNG FASE
// =================================================

PhaseInfo calculatePhase(
  unsigned long elapsedTime,
  unsigned long greenOffset
) {
  unsigned long localTime = (elapsedTime + CYCLE_TIME - greenOffset) % CYCLE_TIME;

  PhaseInfo result;

  // HIJAU
  if (localTime < GREEN_TIME) {
    result.phase = GREEN_PHASE;

    result.remainingMs = GREEN_TIME - localTime;
  }

  // KUNING SETELAH HIJAU
  else if (localTime < GREEN_TIME + YELLOW_TIME) {
    result.phase = YELLOW_PHASE;

    result.remainingMs = GREEN_TIME + YELLOW_TIME - localTime;
  }

  // MERAH
  else if (localTime < GREEN_TIME + YELLOW_TIME + RED_TIME) {
    result.phase = RED_PHASE;

    result.remainingMs = GREEN_TIME + YELLOW_TIME + RED_TIME - localTime;
  }

  // KUNING SEBELUM HIJAU
  else {
    result.phase = YELLOW_PHASE;

    result.remainingMs = CYCLE_TIME - localTime;
  }

  return result;
}


// =================================================
// 9. MENGATUR LAMPU
// =================================================

void applyTrafficLight(
  int index,
  LightPhase phase
) {
  TrafficLight& light = lights[index];

  digitalWrite(light.red, phase == RED_PHASE? HIGH : LOW);

  digitalWrite(light.yellow, phase == YELLOW_PHASE ? HIGH : LOW);

  digitalWrite(light.green, phase == GREEN_PHASE? HIGH : LOW);
}


// =================================================
// 10. MENGHITUNG DETIK
// =================================================

int calculateRemainingSeconds(
  unsigned long remainingMs
) {
  return (remainingMs + 999UL) / 1000UL;
}


// =================================================
// 11. UPDATE DISPLAY
// =================================================

void updateDisplay(
  int index,
  unsigned long remainingMs
) {
  int remainingSeconds = calculateRemainingSeconds(remainingMs);

  if (remainingSeconds != lastDisplayValue[index]) {
    lights[index].display->showNumberDec( remainingSeconds, true, 2, 2);

    lastDisplayValue[index] = remainingSeconds;
  }
}


// =================================================
// 12. NAMA FASE
// =================================================

const char* getPhaseName(
  LightPhase phase
) {
  switch (phase) {

    case RED_PHASE:
      return "MERAH";

    case YELLOW_PHASE:
      return "KUNING";

    case GREEN_PHASE:
      return "HIJAU";

    default:
      return "UNKNOWN";

  }
}


// =================================================
// 13. LOG SEDERHANA
// =================================================

void printSimpleLog(
  int index,
  LightPhase phase
) {
  Serial.print(lights[index].name);

  Serial.print(" -> ");

  Serial.println(getPhaseName(phase));
}


// =================================================
// 14. UPDATE SISTEM
// =================================================

void updateSystem() {
  unsigned long currentTime = millis();

  unsigned long elapsedTime = (currentTime - cycleStartTime) % CYCLE_TIME;

  for (int i = 0; i < TOTAL_DIRECTIONS; i++) {
    // Hitung fase
    PhaseInfo phase = calculatePhase(elapsedTime, GREEN_OFFSET[i]);

    // Update lampu
    applyTrafficLight(i, phase.phase);

    // Update display
    updateDisplay(i, phase.remainingMs);

    // Log hanya ketika fase berubah
    if (lastPhase[i] != (int)phase.phase) {
      lastPhase[i] = (int)phase.phase;

      printSimpleLog(i, phase.phase);
    }
  }
}


// =================================================
// 15. SETUP
// =================================================

void setup() {
  Serial.begin(115200);

  // -----------------------------------------------
  // Traffic light
  // -----------------------------------------------

  for (int i = 0; i < TOTAL_DIRECTIONS; i++) {
    pinMode(lights[i].red, OUTPUT);
    pinMode(lights[i].yellow, OUTPUT);
    pinMode(lights[i].green, OUTPUT);
  }

  // -----------------------------------------------
  // TM1637
  // -----------------------------------------------

  for (int i = 0; i < TOTAL_DIRECTIONS; i++) {
    lights[i].display->setBrightness(5, true);

    lights[i].display->clear();
  }

  // -----------------------------------------------
  // Kondisi awal
  // -----------------------------------------------

  for (int i = 0; i < TOTAL_DIRECTIONS; i++) {
    digitalWrite(lights[i].red, HIGH);
    digitalWrite(lights[i].yellow, LOW);
    digitalWrite(lights[i].green, LOW);
  }

  cycleStartTime = millis();

  Serial.println("Traffic Light START");

  updateSystem();
}


// =================================================
// 16. LOOP
// =================================================

void loop() {
  updateSystem();
}