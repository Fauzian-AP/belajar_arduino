/*
  ESP32 Traffic Light + IR Sensor

  Urutan: MERAH -> KUNING -> HIJAU -> KUNING -> MERAH

  Ketentuan:
  - Merah menyala minimal 3 detik dan menunggu objek.
  - Kuning menyala selama 1 detik pada setiap transisi.
  - Hijau menyala minimal 3 detik.
  - Setelah hijau, tunggu kondisi kosong selama 2 detik.

  Asumsi:
  - Sensor IR aktif-LOW.
  - LED aktif-HIGH.
*/

// ==================== PIN ====================

const int LED_RED = 25;
const int LED_YELLOW = 26;
const int LED_GREEN = 27;
const int SENSOR_IR = 34;

// ==================== KONFIGURASI WAKTU ====================

const unsigned long MIN_RED = 3000;
const unsigned long MIN_GREEN = 3000;
const unsigned long YELLOW_DURATION = 1000;
const unsigned long EMPTY_DURATION = 2000;

// ==================== STATUS LAMPU ====================

enum TrafficState {
  RED_WAIT,
  YELLOW_TO_GREEN,
  GREEN_WAIT,
  YELLOW_TO_RED
};

TrafficState currentState = RED_WAIT;

// ==================== PENCATATAN WAKTU ====================

unsigned long stateStartTime = 0;   // Waktu terjadi perubahan status Traffic Light (ms)
unsigned long lastObjectDetectionTime = 0;   // Waktu terakhir objek terdeteksi (ms)

bool emptyTimerStarted = false;   // Menandakan penghitungan waktu tanpa objek dimulai

// ==================== SENSOR ====================

// Memeriksa apakah sensor IR mendeteksi objek.
bool isObjectDetected() {
  return digitalRead(SENSOR_IR) == LOW;
}

// ==================== PENGATURAN LAMPU ====================

// Mematikan semua LED, kemudian menyalakan 1 LED pilihan.
void switchOnLight(int lampPin) {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);

  digitalWrite(lampPin, HIGH);
}

// ==================== PERUBAHAN STATUS ====================

// Mengubah status lampu dan mencatat waktu transisi.
void changeState(TrafficState newState) {
  currentState = newState;
  stateStartTime = millis();

  switch (currentState) {
    case RED_WAIT:
      switchOnLight(LED_RED);
      Serial.println("MERAH");
      break;

    case YELLOW_TO_GREEN:
      switchOnLight(LED_YELLOW);
      Serial.println("KUNING -> HIJAU");
      break;

    case GREEN_WAIT:
      switchOnLight(LED_GREEN);
      Serial.println("HIJAU");

      emptyTimerStarted = false;   // Reset timer kondisi kosong saat hijau dimulai.
      break;

    case YELLOW_TO_RED:
      switchOnLight(LED_YELLOW);
      Serial.println("KUNING -> MERAH");
      break;
  }
}

// ==================== SETUP ====================

void setup() {
  Serial.begin(115200);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(SENSOR_IR, INPUT);

  // Mulai dengan lampu merah.
  changeState(RED_WAIT);
}

// ==================== LOOP ====================

void loop() {
  const unsigned long now = millis();
  const bool objectDetected = isObjectDetected();

  switch (currentState) {
    case RED_WAIT:
      // Tunggu minimal 3 detik dan pastikan objek terdeteksi.
      if (now - stateStartTime >= MIN_RED && objectDetected) {
        changeState(YELLOW_TO_GREEN);
      }
      break;

    case YELLOW_TO_GREEN:
      // Transisi dari merah ke hijau.
      if (now - stateStartTime >= YELLOW_DURATION) {
        changeState(GREEN_WAIT);
      }
      break;

    case GREEN_WAIT:
      // Pastikan hijau menyala minimal 3 detik.
      if (now - stateStartTime < MIN_GREEN) {
        break;
      }

      // Catat waktu terakhir objek terdeteksi.
      if (objectDetected) {
        lastObjectDetectionTime = now;
        emptyTimerStarted = true;
      } else if (!emptyTimerStarted) {
        // Mulai menghitung saat kondisi kosong.
        lastObjectDetectionTime = now;
        emptyTimerStarted = true;
      }

      // Kembali menuju merah setelah kosong selama 2 detik.
      if (emptyTimerStarted && !objectDetected && now - lastObjectDetectionTime >= EMPTY_DURATION) {
        changeState(YELLOW_TO_RED);
      }
      break;

    case YELLOW_TO_RED:
      // Transisi dari hijau ke merah.
      if (now - stateStartTime >= YELLOW_DURATION) {
        changeState(RED_WAIT);
      }
      break;
  }
}
