/*
  ESP32 + Water Level Sensor + Traffic Light + Buzzer

  Logika:
  - Air aman    : LED hijau menyala, buzzer mati.
  - Air bahaya  : LED merah menyala, buzzer berbunyi.

  Sensor analog:
  - Dibaca melalui GPIO34.
  - Nilai ADC ESP32 umumnya 0 sampai 4095.
  - Ambang perlu disesuaikan berdasarkan pengujian sensor.
*/

// ==================== PIN ====================

const int LED_RED = 25;   // 25
const int LED_GREEN = 27;   // 27

const int WATER_SENSOR_PIN = 33;
const int BUZZER_PIN = 18;

// ==================== AMBANG SENSOR ====================

// Sesuaikan setelah melihat hasil Serial Monitor.
const int DANGER_THRESHOLD = 2000;
const int SAFE_THRESHOLD = 1500;

// Jumlah sampel untuk meredam fluktuasi pembacaan.
const int SAMPLE_COUNT = 10;

// Status bahaya air saat ini.
bool waterDanger = false;

// ==================== PEMBACAAN SENSOR ====================

// Membaca beberapa sampel lalu menghitung rata-ratanya.
int readWaterLevel() {
  long total = 0;

  for (int i = 0; i < SAMPLE_COUNT; i++) {
    total += analogRead(WATER_SENSOR_PIN);
    delay(5);
  }

  return total / SAMPLE_COUNT;
}

// Menentukan status air dengan hysteresis.
// Ambang bahaya dan aman dibuat berbeda agar indikator tidak mudah berubah-ubah di sekitar batas.
bool isWaterDanger(int sensorValue) {
  if (sensorValue >= DANGER_THRESHOLD) {
    waterDanger = true;
  } else if (sensorValue <= SAFE_THRESHOLD) {
    waterDanger = false;
  }

  return waterDanger;
}

// ==================== INDIKATOR ====================

// Kondisi bahaya: merah + buzzer.
// Kondisi aman: hijau + buzzer mati.
void updateIndicators(bool danger) {
  digitalWrite(LED_RED, danger ? HIGH : LOW);
  digitalWrite(LED_GREEN, danger ? LOW : HIGH);
  digitalWrite(BUZZER_PIN, danger ? HIGH : LOW);
}

// ==================== SETUP ====================

void setup() {
  Serial.begin(115200);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // ESP32 menggunakan ADC 12-bit: 0 sampai 4095.
  analogReadResolution(12);

  // Kondisi awal: aman.
  updateIndicators(false);

  Serial.println("Sistem pemantau ketinggian air dimulai.");
}

// ==================== LOOP ====================

void loop() {
  const int waterValue = readWaterLevel();
  const bool danger = isWaterDanger(waterValue);

  updateIndicators(danger);

  // Tampilkan data untuk membantu kalibrasi.
  Serial.print("Nilai sensor air: ");
  Serial.print(waterValue);

  if (danger) {
    Serial.println(" | BAHAYA - LED MERAH + BUZZER");
  } else {
    Serial.println(" | AMAN - LED HIJAU");
  }

  delay(200);
}
