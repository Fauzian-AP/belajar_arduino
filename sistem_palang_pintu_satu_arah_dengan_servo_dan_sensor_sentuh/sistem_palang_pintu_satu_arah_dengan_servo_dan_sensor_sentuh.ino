#include <Servo.h>


/*
=========================================================
   PALANG OTOMATIS: Arduino Uno + Sensor Touch + Servo
=========================================================

ALUR: Sensor disentuh & ditahan 1s -> Servo naik perlahan -> Servo tetap terbuka selama 2s -> Servo menutup kembali

Note:
- Sentuhan kurang dari 1 detik diabaikan.

PEMASANGAN:
- Touch Sensor : SIG -> D2, VCC -> 5V, GND -> GND
- Servo MG90S  : Signal -> D5, VCC -> 5V, GND -> GND
*/


// ==================================================
// 1. KONFIGURASI PIN
// ==================================================

const int SENSOR_PIN = 2;
const int SERVO_PIN  = 5;


// ==================================================
// 2. POSISI SERVO
// ==================================================

const int CLOSED_POSITION = 90;   // Palang mendatar
const int OPEN_POSITION   = 160;  // Palang tegak


// ==================================================
// 3. PENGATURAN WAKTU
// ==================================================

// Jeda setiap perpindahan 1 derajat. Semakin besar nilainya, semakin lambat servo bergerak.

const int OPEN_DELAY = 10;   // Kecepatan saat membuka
const int CLOSE_DELAY = 20;   // Kecepatan saat menutup

const unsigned long OPEN_HOLD_TIME  = 2000UL;   // Lama palang berada pada posisi terbuka.
const unsigned long TOUCH_HOLD_TIME = 1000UL;   // Lama sensor harus disentuh agar dianggap valid.
const unsigned long HORN_SETUP_TIME = 5000UL;   // Waktu untuk memasang horn servo saat startup.


// ==================================================
// 4. OBJEK & STATUS
// ==================================================

Servo barrierServo;

int currentPosition = CLOSED_POSITION;


// ==================================================
// 5. GERAKKAN SERVO
// ==================================================

void moveServoTo(int targetPosition, int delayTime) {
  // Servo bergerak 1 derajat setiap langkah sampai mencapai posisi tujuan.

  while (currentPosition != targetPosition) {
    if (currentPosition < targetPosition) {
      currentPosition++;
    }

    else {
      currentPosition--;
    }

    barrierServo.write(currentPosition);

    delay(delayTime);
  }
}


// ==================================================
// 6. MEMERIKSA SENTUHAN SENSOR
// ==================================================

bool isTouchValid() {
  // Waktu mulainya.
  unsigned long startTime = millis();

  // Sensor harus tetap HIGH selama TOUCH_HOLD_TIME.
  while (digitalRead(SENSOR_PIN) == HIGH) {
    if (millis() - startTime >= TOUCH_HOLD_TIME) {
      return true;
    }

    delay(10);
  }

  // Sensor dilepas sebelum waktunya.
  return false;
}


// ==================================================
// 7. MEMBUKA DAN MENUTUP PALANG
// ==================================================

void openAndCloseBarrier() {
  Serial.println("Palang NAIK");
  moveServoTo(OPEN_POSITION, OPEN_DELAY);

  Serial.println("Palang terbuka, menunggu...");
  delay(OPEN_HOLD_TIME);

  Serial.println("Palang TURUN");
  moveServoTo(CLOSED_POSITION, CLOSE_DELAY);

  Serial.println("Palang tertutup.");
}


// ==================================================
// 8. SETUP
// ==================================================

void setup() {
  Serial.begin(9600);

  // Sensor Sentuh.
  pinMode(SENSOR_PIN, INPUT);

  // Servo.
  barrierServo.attach(SERVO_PIN);

  // Posisi awal palang.
  barrierServo.write(CLOSED_POSITION);
  currentPosition = CLOSED_POSITION;

  // Persiapan pemasangan horn servo.
  Serial.println("Servo berada di posisi TUTUP.");
  Serial.println("Pasang horn servo sekarang (5 detik)...");
  delay(HORN_SETUP_TIME);

  Serial.println("Sistem siap.");
  Serial.println("Sentuh dan tahan sensor untuk membuka palang.");
}


// ==================================================
// 9. LOOP
// ==================================================

void loop() {
  // Cek apakah sensor sedang disentuh.
  if (digitalRead(SENSOR_PIN) == HIGH) {
    // Periksa apakah sentuhan cukup lama.
    if (isTouchValid()) {
      Serial.println("Sentuhan valid -> palang NAIK");

      // Proses Buka Tutup Palang.
      openAndCloseBarrier();

      // Tunggu sampai jari dilepas agar palang tidak membuka kembali.
      while (digitalRead(SENSOR_PIN) == HIGH) {
        delay(10);
      }

      // Debounce.
      delay(500);
    }

    else {
      Serial.println("Sentuhan terlalu singkat, diabaikan.");
    }
  }
}