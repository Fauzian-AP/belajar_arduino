#include <Servo.h>


/*
====================================================================
   PALANG OTOMATIS 2 ARAH: Arduino Uno + 2 Touch Sensor + 2 Servo
====================================================================

ALUR:
- ARAH MASUK: Sensor disentuh & ditahan 1s -> Servo naik perlahan -> Servo tetap terbuka selama 2s -> Servo menutup kembali
- ARAH KELUAR: Sensor disentuh & ditahan 1s -> Servo naik perlahan -> Servo tetap terbuka selama 2s -> Servo menutup kembali

Note:
- Sentuhan kurang dari 1 detik diabaikan.
- Setiap arah memiliki 1 Touch Sensor dan 1 Servo.

PEMASANGAN:
- TOUCH SENSOR MASUK: SIG -> D2, VCC -> 5V, GND -> GND
- TOUCH SENSOR KELUAR: SIG -> D8, VCC -> 5V, GND -> GND
- SERVO MG90S MASUK: Signal -> D5, VCC -> 5V, GND -> GND
- SERVO MG90S KELUAR: Signal -> D9, VCC -> 5V, GND -> GND
*/


// ==================================================
// 1. KONFIGURASI PIN
// ==================================================

const int SENSOR_MASUK_PIN = 2;
const int SENSOR_KELUAR_PIN = 8;

const int SERVO_MASUK_PIN = 5;
const int SERVO_KELUAR_PIN = 9;


// ==================================================
// 2. POSISI SERVO
// ==================================================

const int CLOSED_POSITION = 90;   // Palang mendatar
const int OPEN_POSITION   = 160;   // Palang tegak


// ==================================================
// 3. PENGATURAN WAKTU
// ==================================================

// Jeda setiap perpindahan 1 derajat.
// Semakin besar nilainya, semakin lambat servo bergerak.

const int OPEN_DELAY  = 10;   // Kecepatan saat membuka
const int CLOSE_DELAY = 20;   // Kecepatan saat menutup

const unsigned long OPEN_HOLD_TIME = 2000UL;   // Lama palang berada pada posisi terbuka.
const unsigned long TOUCH_HOLD_TIME = 1000UL;   // Lama sensor harus disentuh agar dianggap valid.
const unsigned long HORN_SETUP_TIME = 5000UL;   // Waktu untuk memasang horn servo saat startup.


// ==================================================
// 4. CLASS PALANG
// ==================================================

class BarrierDirection {
private:
  Servo servo;

  int sensorPin;
  int servoPin;

  int currentPosition;

  const char* directionName;

public:
  // ==================================================
  // CONSTRUCTOR
  // ==================================================

  BarrierDirection(
    int sensorPin,
    int servoPin,
    const char* directionName
  ) {
    this->sensorPin = sensorPin;
    this->servoPin = servoPin;
    this->directionName = directionName;

    currentPosition = CLOSED_POSITION;
  }


  // ==================================================
  // INISIALISASI PALANG
  // ==================================================

  void begin() {
    pinMode(sensorPin, INPUT);

    servo.attach(servoPin);

    servo.write(CLOSED_POSITION);

    currentPosition = CLOSED_POSITION;
  }


  // ==================================================
  // GERAKKAN SERVO
  // ==================================================

  void moveTo(int targetPosition, int delayTime) {
    // Servo bergerak 1 derajat setiap langkah sampai mencapai posisi tujuan.

    while (currentPosition != targetPosition) {
      if (currentPosition < targetPosition) {
        currentPosition++;
      }

      else {
        currentPosition--;
      }

      servo.write(currentPosition);

      delay(delayTime);
    }
  }


  // ==================================================
  // PERIKSA SENTUHAN
  // ==================================================

  bool isTouchValid() {
    // Waktu mulainya.
    unsigned long startTime = millis();

    // Sensor harus tetap HIGH selama TOUCH_HOLD_TIME.
    while (digitalRead(sensorPin) == HIGH) {
      if (millis() - startTime >= TOUCH_HOLD_TIME) {
        return true;
      }

      delay(10);
    }

    // Sensor dilepas sebelum waktunya.
    return false;
  }


  // ==================================================
  // BUKA DAN TUTUP PALANG
  // ==================================================

  void openAndClose() {
    Serial.print(directionName);
    Serial.println(" -> Palang NAIK");

    moveTo(OPEN_POSITION, OPEN_DELAY);

    Serial.print(directionName);
    Serial.println(" -> Palang terbuka");

    delay(OPEN_HOLD_TIME);

    Serial.print(directionName);
    Serial.println(" -> Palang TURUN");

    moveTo(CLOSED_POSITION, CLOSE_DELAY);

    Serial.print(directionName);
    Serial.println(" -> Palang tertutup");
  }


  // ==================================================
  // PERIKSA SENSOR & JALANKAN PALANG
  // ==================================================

  void update() {
    // Cek apakah sensor sedang disentuh.
    if (digitalRead(sensorPin) == HIGH) {
      // Periksa apakah sentuhan cukup lama.
      if (isTouchValid()) {
        Serial.print(directionName);
        Serial.println(" -> Sentuhan valid, membuka palang");

        // Proses Buka Tutup Palang.
        openAndClose();

        // Tunggu sampai jari dilepas agar palang tidak membuka kembali.
        while (digitalRead(sensorPin) == HIGH) {
          delay(10);
        }

        // Debounce.
        delay(500);
      }

      else {
        Serial.print(directionName);
        Serial.println(" -> Sentuhan terlalu singkat");
      }
    }
  }
};


// ==================================================
// 5. OBJEK & STATUS
// ==================================================

BarrierDirection masuk(SENSOR_MASUK_PIN, SERVO_MASUK_PIN, "MASUK");

BarrierDirection keluar(SENSOR_KELUAR_PIN, SERVO_KELUAR_PIN, "KELUAR");


// ==================================================
// 6. SETUP
// ==================================================

void setup() {
  Serial.begin(9600);

  // Inisialisasi palang.
  masuk.begin();
  keluar.begin();

  // Persiapan pemasangan horn servo.
  Serial.println("Servo berada di posisi TUTUP.");
  Serial.println("Pasang horn servo sekarang...");

  delay(HORN_SETUP_TIME);

  Serial.println("Sistem siap.");

  Serial.println("Sensor MASUK -> Arah MASUK");
  Serial.println("Sensor KELUAR -> Arah KELUAR");
}


// ==================================================
// 7. LOOP
// ==================================================

void loop() {
  // Periksa palang arah MASUK.
  masuk.update();

  // Periksa palang arah KELUAR.
  keluar.update();
}