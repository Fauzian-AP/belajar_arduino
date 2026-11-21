/*
================================================
       ESP32 + VIBRATION SENSOR + BUZZER
================================================

FUNGSI:
- Mendeteksi getaran dari sensor.
- Saat getaran terdeteksi, alarm aktif selama 8 detik.
- Buzzer berbunyi secara berkedip:
    ON  = 200 ms
    OFF = 150 ms
- Jika getaran baru terdeteksi selama alarm aktif,
  waktu alarm diperpanjang kembali menjadi 8 detik.
*/


// =================================================
// 1. PIN
// =================================================

const int SENSOR_PIN = 34;
const int BUZZER_PIN = 25;


// =================================================
// 2. KONFIGURASI WAKTU
// =================================================

const unsigned long ALARM_DURATION = 5000UL;

const unsigned long BEEP_ON_TIME  = 200UL;
const unsigned long BEEP_OFF_TIME = 200UL;


// =================================================
// 3. VARIABEL STATUS
// =================================================

unsigned long vibrationTime = 0;
unsigned long beepTime = 0;

bool alarmActive = false;
bool buzzerState = false;


// =================================================
// 4. SETUP
// =================================================

void setup() {
  Serial.begin(115200);

  pinMode(SENSOR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Pastikan buzzer mati saat awal
  digitalWrite(BUZZER_PIN, LOW);
}


// =================================================
// 5. MEMATIKAN ALARM
// =================================================

void stopAlarm() {
  alarmActive = false;
  buzzerState = false;

  digitalWrite(BUZZER_PIN, LOW);
}


// =================================================
// 6. UPDATE BUZZER
// =================================================

void updateBuzzer(unsigned long now) {
  // Interval bunyi
  unsigned long beepInterval = buzzerState ? BEEP_ON_TIME : BEEP_OFF_TIME;

  if (now - beepTime >= beepInterval) {
    beepTime = now;
    buzzerState = !buzzerState;

    digitalWrite(BUZZER_PIN, buzzerState ? HIGH : LOW);
  }
}


// =================================================
// 7. UPDATE SISTEM
// =================================================

void updateSystem() {
  // Ambil waktu sekarang
  unsigned long now = millis();

  // Deteksi getaran
  if (digitalRead(SENSOR_PIN) == HIGH) {
    vibrationTime = now;

    // Jika alarm sebelumnya mati, mulai pola buzzer dari awal.
    if (!alarmActive) {
      alarmActive = true;
      buzzerState = false;
      beepTime = now;

      digitalWrite(BUZZER_PIN, LOW);
    }
  }

  // Cek durasi alarm
  if (alarmActive && now - vibrationTime >= ALARM_DURATION) {
    stopAlarm();
  }

  // Update buzzer
  if (alarmActive) {
    updateBuzzer(now);
  }
}


// =================================================
// 8. LOOP
// =================================================

void loop() {
  updateSystem();
}