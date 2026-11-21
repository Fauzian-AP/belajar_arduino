/*
  ESP32 Traffic Light + IR Sensor + Buzzer

  Urutan: MERAH -> KUNING -> HIJAU -> KUNING -> MERAH

  Ketentuan:
  - Merah menyala minimal 3 detik dan menunggu objek.
  - Kuning menyala selama 1 detik pada setiap transisi.
  - Hijau menyala minimal 3 detik.
  - Setelah hijau, tunggu kondisi kosong selama 2 detik.
  - Buzzer menyala saat hijau aktif dan objek terdeteksi.

  Asumsi:
  - Sensor IR aktif-LOW.
  - LED aktif-HIGH.
  - Buzzer aktif-HIGH.
*/

// ==================== PIN ====================

const int LED_RED = 25;
const int LED_YELLOW = 26;
const int LED_GREEN = 27;
const int SENSOR_IR = 34;
const int BUZZER_PIN = 18;

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

// ==================== VARIABEL WAKTU ====================

unsigned long stateStartTime = 0;
unsigned long lastObjectDetectionTime = 0;

bool emptyTimerStarted = false;

// ==================== SENSOR ====================

// Memeriksa apakah sensor mendeteksi objek.
bool isObjectDetected() {
  return digitalRead(SENSOR_IR) == LOW;
}

// ==================== LAMPU ====================

// Mematikan semua LED, lalu menyalakan 1 LED yang dipilih.
void switchOnLight(int lampPin) {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);

  digitalWrite(lampPin, HIGH);
}

// ==================== PERUBAHAN STATUS ====================

// Memperbarui status, waktu, dan lampu lalu lintas.
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

      emptyTimerStarted = false;   // Timer kondisi kosong dimulai kembali.
      break;

    case YELLOW_TO_RED:
      switchOnLight(LED_YELLOW);
      Serial.println("KUNING -> MERAH");
      break;
  }
}

// ==================== BUZZER ====================

// Buzzer aktif hanya saat hijau dan objek terdeteksi.
void updateAlarm(bool objectDetected) {
  const bool alarmActive = currentState == GREEN_WAIT && objectDetected;

  digitalWrite(BUZZER_PIN, alarmActive ? HIGH : LOW);
}

// ==================== SETUP ====================

void setup() {
  Serial.begin(115200);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  pinMode(SENSOR_IR, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Kondisi awal perangkat.
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // Mulai dari lampu merah.
  changeState(RED_WAIT);
}

// ==================== LOOP ====================

void loop() {
  const unsigned long now = millis();
  const bool objectDetected = isObjectDetected();

  switch (currentState) {
    case RED_WAIT:
      // Menunggu minimal 3 detik dan objek terdeteksi.
      if (now - stateStartTime >= MIN_RED && objectDetected) {
        changeState(YELLOW_TO_GREEN);
      }
      break;

    case YELLOW_TO_GREEN:
      // Menunggu durasi transisi kuning.
      if (now - stateStartTime >= YELLOW_DURATION) {
        changeState(GREEN_WAIT);
      }
      break;

    case GREEN_WAIT:
      // Hijau harus menyala minimal 3 detik.
      if (now - stateStartTime < MIN_GREEN) {
        break;
      }

      // Perbarui waktu jika objek terdeteksi.
      if (objectDetected) {
        lastObjectDetectionTime = now;
        emptyTimerStarted = true;
      } else if (!emptyTimerStarted) {
        // Mulai timer ketika kondisi kosong.
        lastObjectDetectionTime = now;
        emptyTimerStarted = true;
      }

      // Kembali menuju merah jika kosong 2 detik.
      if (emptyTimerStarted && !objectDetected && now - lastObjectDetectionTime >= EMPTY_DURATION) {
        changeState(YELLOW_TO_RED);
      }
      break;

    case YELLOW_TO_RED:
      // Menunggu durasi transisi kuning.
      if (now - stateStartTime >= YELLOW_DURATION) {
        changeState(RED_WAIT);
      }
      break;
  }

  // Perbarui buzzer berdasarkan status terbaru.
  updateAlarm(objectDetected);
}
