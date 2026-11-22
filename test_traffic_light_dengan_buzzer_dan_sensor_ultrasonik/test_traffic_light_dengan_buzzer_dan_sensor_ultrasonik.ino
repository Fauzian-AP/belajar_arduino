/*
  ESP32 + HC-SR04 + Traffic Light + Buzzer

  Alur:
  MERAH -> HIJAU -> MERAH

  Ketentuan:
  - Merah menyala minimal 3 detik.
  - Hijau menyala minimal 3 detik.
  - Objek terdeteksi jika jarak <= 20 cm.
  - Kembali ke merah setelah objek tidak terdeteksi selama 2 detik.
  - Buzzer aktif saat hijau dan objek terdeteksi.
  - Pengukuran ultrasonik dilakukan sekitar setiap 60 ms.
*/

// ==================== PIN ====================

const int LED_RED = 25;
const int LED_GREEN = 27;

const int TRIG_PIN = 32;
const int ECHO_PIN = 33;

const int BUZZER_PIN = 18;

// ==================== KONFIGURASI ====================

// Semua durasi waktu menggunakan milidetik.
const unsigned long MIN_RED = 3000;
const unsigned long MIN_GREEN = 3000;
const unsigned long EMPTY_DURATION = 2000;

const unsigned long SENSOR_INTERVAL = 60;
const unsigned long ECHO_TIMEOUT_US = 25000;

// Batas deteksi objek dalam sentimeter.
const float DETECTION_DISTANCE_CM = 20.0f;

// ==================== STATUS LAMPU ====================

enum TrafficState {
  RED_WAIT,
  GREEN_WAIT
};

TrafficState currentState = RED_WAIT;

// ==================== PENCATATAN WAKTU ====================

unsigned long stateStartTime = 0;
unsigned long lastObjectDetectionTime = 0;
unsigned long lastSensorReadTime = 0;

bool emptyTimerStarted = false;

// Nilai -1 berarti jarak belum berhasil terbaca.
float distanceCm = -1.0f;

// ==================== SENSOR ULTRASONIK ====================

// Membaca jarak objek menggunakan HC-SR04 dan mengembalikan -1 jika pantulan tidak terbaca.
float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long duration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);

  if (duration == 0) {
    return -1.0f;
  }

  // Kecepatan suara sekitar 0,0343 cm per mikrodetik.
  // Dibagi 2 karena suara menempuh perjalanan pergi-pulang.
  return (duration * 0.0343f) / 2.0f;
}

// Memeriksa apakah jarak masuk ke batas deteksi.
bool isObjectDetected(float distance) {
  return distance > 0.0f && distance <= DETECTION_DISTANCE_CM;
}

// ==================== PENGATURAN LAMPU ====================

// Hanya ada dua lampu indikator: merah dan hijau.
void switchOnLight(int lampPin) {
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);

  digitalWrite(lampPin, HIGH);
}

// ==================== PERUBAHAN STATUS ====================

void changeState(TrafficState newState) {
  currentState = newState;
  stateStartTime = millis();

  switch (currentState) {
    case RED_WAIT:
      switchOnLight(LED_RED);
      emptyTimerStarted = false;
      Serial.println("LAMPU MERAH");
      break;

    case GREEN_WAIT:
      switchOnLight(LED_GREEN);
      emptyTimerStarted = false;
      Serial.println("LAMPU HIJAU");
      break;
  }
}

// ==================== BUZZER ====================

// Buzzer aktif hanya ketika lampu hijau menyala dan objek terdeteksi dalam jarak yang ditentukan.
void updateBuzzer(bool objectDetected) {
  const bool buzzerActive = currentState == GREEN_WAIT && objectDetected;

  digitalWrite(BUZZER_PIN, buzzerActive ? HIGH : LOW);
}

// ==================== SETUP ====================

void setup() {
  Serial.begin(115200);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  changeState(RED_WAIT);
}

// ==================== LOOP ====================

void loop() {
  const unsigned long now = millis();

  // Baca sensor dengan jeda antar-pengukuran.
  if (now - lastSensorReadTime >= SENSOR_INTERVAL) {
    lastSensorReadTime = now;
    distanceCm = readDistanceCm();
  }

  const bool objectDetected = isObjectDetected(distanceCm);

  switch (currentState) {
    case RED_WAIT:
      // Beralih langsung ke hijau tanpa lampu kuning.
      if (now - stateStartTime >= MIN_RED && objectDetected) {
        changeState(GREEN_WAIT);
      }
      break;

    case GREEN_WAIT:
      // Hijau harus menyala minimal 3 detik.
      if (now - stateStartTime < MIN_GREEN) {
        break;
      }

      // Jika objek terdeteksi, mulai ulang timer kosong.
      if (objectDetected) {
        lastObjectDetectionTime = now;
        emptyTimerStarted = true;
      } else if (!emptyTimerStarted) {
        // Mulai menghitung ketika objek tidak terdeteksi.
        lastObjectDetectionTime = now;
        emptyTimerStarted = true;
      }

      // Kembali langsung ke merah setelah kosong 2 detik.
      if (emptyTimerStarted && !objectDetected && now - lastObjectDetectionTime >= EMPTY_DURATION) {
        changeState(RED_WAIT);
      }
      break;
  }

  updateBuzzer(objectDetected);

  // Tampilkan hasil pembacaan setiap 250 ms.
  static unsigned long lastPrintTime = 0;

  if (now - lastPrintTime >= 250) {
    lastPrintTime = now;

    if (distanceCm > 0.0f) {
      Serial.print("Jarak: ");
      Serial.print(distanceCm, 1);
      Serial.println(" cm");
    } else {
      Serial.println("Jarak: tidak terbaca");
    }
  }
}
