/*
  Flame Alarm - ESP32

  Normal : LED hijau ON, buzzer OFF
  Api    : LED merah ON, buzzer bip cepat
           (bertahan 3 detik setelah api hilang)

  Pin:
    Flame sensor DO -> 34
    Buzzer aktif    -> 23
    LED merah       -> 25
    LED hijau       -> 27
*/

// ===== Pin =====
const int PIN_FLAME  = 34;  
const int PIN_BUZZER = 18;
const int PIN_RED    = 25;
const int PIN_GREEN  = 27;

// ===== Konfigurasi =====
const int           FIRE_LEVEL    = LOW;   // LOW = api terdeteksi
const unsigned long HOLD_TIME_MS  = 3000;  // alarm bertahan setelah api hilang
const unsigned long BEEP_PERIOD   = 200;   // periode bip (ms)

// ===== State =====
unsigned long lastFireMs = 0;
bool alarmActive = false;

bool isFireDetected() {
  return digitalRead(PIN_FLAME) == FIRE_LEVEL;
}

void setOutputs(bool red, bool green, bool buzzer) {
  digitalWrite(PIN_RED, red);
  digitalWrite(PIN_GREEN, green);
  digitalWrite(PIN_BUZZER, buzzer);
}

bool beepPattern() {
  return (millis() % BEEP_PERIOD) < (BEEP_PERIOD / 2);
}

void updateAlarmState() {
  unsigned long now = millis();

  if (isFireDetected()) {
    lastFireMs = now;
    if (!alarmActive) {
      alarmActive = true;
      Serial.println("API TERDETEKSI");
    }
  } else if (alarmActive && now - lastFireMs >= HOLD_TIME_MS) {
    alarmActive = false;
    Serial.println("NORMAL");
  }
}

void updateOutputs() {
  if (alarmActive) {
    setOutputs(HIGH, LOW, beepPattern());
  } else {
    setOutputs(LOW, HIGH, LOW);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_FLAME, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);

  updateOutputs();
  Serial.println("NORMAL");
}

void loop() {
  updateAlarmState();
  updateOutputs();
  delay(10);
}