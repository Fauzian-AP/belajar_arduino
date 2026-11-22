/*
===================================================================
   PENDETEKSI GEMPA SEDERHANA: ESP32 + VIBRATION SENSOR + BUZZER
===================================================================

ALUR: Sensor menunggu getaran -> Terjadi getaran -> Alarm Buzzer aktif selama 5s -> Jika ada getaran baru, maka alarm diperpanjang 8s -> Jika tidak, maka alarm mati

NOTE:
- Buzzer berbunyi secara berkedip: ON (200ms), OFF (150ms).

PEMASANGAN:
- Vibration Sensor : DO -> D34, VCC -> 3V3, GND -> GND
- Buzzer  : Signal -> D25, GND -> GND
*/


// =================================================
// 1. KONFIGURASI PIN
// =================================================

const int SENSOR_PIN = 34;
const int BUZZER_PIN = 25;


// =================================================
// 2. KONFIGURASI WAKTU
// =================================================

const unsigned long ALARM_DURATION = 5000UL;   // Durasi alarm & waktu tunngu jika ada getaran baru

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