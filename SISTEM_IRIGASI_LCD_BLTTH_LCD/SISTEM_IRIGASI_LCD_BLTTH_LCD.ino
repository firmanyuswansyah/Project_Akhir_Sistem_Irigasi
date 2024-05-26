#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inisialisasi LCD dengan alamat I2C (0x27 atau 0x3F, tergantung pada modul LCD Anda)
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int sensorPin = A0;        // Pin analog sensor kelembaban
const int relayPin = 10;         // Pin digital modul relai
const int bluetoothTxPin = 2;    // Pin TX modul Bluetooth
const int ledPin1 = 11;          // Pin LED 1
const int ledPin2 = 12;          // Pin LED 2

int moistureValue;               // Nilai sensor kelembaban
int moistureThreshold = 800;     // Nilai ambang batas kelembaban
bool pumpOn = false;             // Status pompa air

void setup() {
  Serial.begin(9600);            // Inisialisasi komunikasi serial
  pinMode(relayPin, OUTPUT);     // Set pin modul relai sebagai output
  pinMode(bluetoothTxPin, OUTPUT);// Set pin TX modul Bluetooth sebagai output
  pinMode(ledPin1, OUTPUT);      // Set pin LED 1 sebagai output
  pinMode(ledPin2, OUTPUT);      // Set pin LED 2 sebagai output

  // Inisialisasi LCD
  lcd.begin(16, 2); // Menentukan jumlah kolom dan baris LCD
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Moisture System");
  delay(2000); // Tampilkan pesan awal selama 2 detik
  lcd.clear();
}

void loop() {
  // Baca nilai sensor kelembaban
  moistureValue = analogRead(sensorPin);

  // Cetak nilai sensor ke monitor serial
  Serial.print("Kelembaban: ");
  Serial.println(moistureValue);

  // Tampilkan nilai sensor di LCD
  lcd.setCursor(0, 0);
  lcd.print("Kelembaban: ");
  lcd.print(moistureValue);
  lcd.print(" RH   "); // Menambahkan satuan (%) dan menghapus sisa karakter sebelumnya

  // Periksa data yang diterima dari Bluetooth
  if (Serial.available() > 0) {
    char command = Serial.read();

    // MATI pompa air jika menerima '1'
    if (command == '1') {
      digitalWrite(relayPin, HIGH);
      pumpOn = true; // Set status pompa air menjadi menyala
      digitalWrite(ledPin1, HIGH); // MATI LED 1
      digitalWrite(ledPin2, HIGH); // MATI LED 2
      Serial.println("Pompa air mati");
      lcd.setCursor(0, 1);
      lcd.print("Pompa: OFF ");
    }

    // NYALAKAN pompa air jika menerima '0'
    else if (command == '0') {
      digitalWrite(relayPin, LOW);
      pumpOn = false; // Set status pompa air menjadi mati
      digitalWrite(ledPin1, LOW); // NYALAKAN LED 1
      digitalWrite(ledPin2, LOW); // NYALAKAN LED 2
      Serial.println("Pompa air menyala");
      lcd.setCursor(0, 1);
      lcd.print("Pompa: ON");
    }
  }

  // Kontrol pompa air dan LED berdasarkan nilai sensor dan ambang batas
  else {
    if (moistureValue < moistureThreshold && !pumpOn) {
      digitalWrite(relayPin, HIGH);
      pumpOn = true; // Set status pompa air menjadi mati
      digitalWrite(ledPin1, HIGH); // mati LED 1
      digitalWrite(ledPin2, HIGH); // mati LED 2
      Serial.println("Pompa air mati (sensor)");
      lcd.setCursor(0, 1);
      lcd.print("Pompa: OFF ");
    } else if (moistureValue >= moistureThreshold && pumpOn) {
      digitalWrite(relayPin, LOW);
      pumpOn = false; // Set status pompa air menjadi hidup
      digitalWrite(ledPin1, LOW); // hidup LED 1
      digitalWrite(ledPin2, LOW); // hidup LED 2
      Serial.println("Pompa air menyala (sensor)");
      lcd.setCursor(0, 1);
      lcd.print("Pompa: ON");
    }
  }

  delay(1000); // Tunggu 1 detik
}


// #include <Wire.h>
// #include <LiquidCrystal_I2C.h>

// // Inisialisasi LCD dengan alamat I2C (0x27 atau 0x3F, tergantung pada modul LCD Anda)
// LiquidCrystal_I2C lcd(0x27, 16, 2);

// const int sensorPin = A0;        // Pin analog sensor kelembaban
// const int relayPin = 10;         // Pin digital modul relai
// const int bluetoothTxPin = 2;    // Pin TX modul Bluetooth
// const int ledPin1 = 11;          // Pin LED 1
// const int ledPin2 = 12;          // Pin LED 2

// int moistureValue;               // Nilai sensor kelembaban
// int moistureThreshold = 800;     // Nilai ambang batas kelembaban
// bool pumpOn = false;             // Status pompa air

// void setup() {
//   Serial.begin(9600);            // Inisialisasi komunikasi serial
//   pinMode(relayPin, OUTPUT);     // Set pin modul relai sebagai output
//   pinMode(bluetoothTxPin, OUTPUT);// Set pin TX modul Bluetooth sebagai output
//   pinMode(ledPin1, OUTPUT);      // Set pin LED 1 sebagai output
//   pinMode(ledPin2, OUTPUT);      // Set pin LED 2 sebagai output

//   // Inisialisasi LCD
//   lcd.begin(16, 2); // Menentukan jumlah kolom dan baris LCD
//   lcd.backlight();
//   lcd.clear();
//   lcd.setCursor(0, 0);
//   lcd.print("Moisture System");
//   delay(2000); // Tampilkan pesan awal selama 2 detik
//   lcd.clear();
// }

// void loop() {
//   // Baca nilai sensor kelembaban
//   moistureValue = analogRead(sensorPin);

//   // Cetak nilai sensor ke monitor serial
//   Serial.print("Kelembaban: ");
//   Serial.println(moistureValue);

//   // Tampilkan nilai sensor di LCD
//   lcd.setCursor(0, 0);
//   lcd.print("Kelembaban: ");
//   lcd.print(moistureValue);
//   lcd.print("   "); // Menghapus sisa karakter sebelumnya

//   // Periksa data yang diterima dari Bluetooth
//   if (Serial.available() > 0) {
//     char command = Serial.read();

//     // Nyalakan pompa air jika menerima '1'
//     if (command == '1') {
//       digitalWrite(relayPin, HIGH);
//       pumpOn = true; // Set status pompa air menjadi menyala
//       digitalWrite(ledPin1, HIGH); // Nyalakan LED 1
//       digitalWrite(ledPin2, HIGH); // Nyalakan LED 2
//       Serial.println("Pompa air mati");
//       lcd.setCursor(0, 1);
//       lcd.print("Pompa: OFF  ");
//     }

//     // Matikan pompa air jika menerima '0'
//     else if (command == '0') {
//       digitalWrite(relayPin, LOW);
//       pumpOn = false; // Set status pompa air menjadi mati
//       digitalWrite(ledPin1, LOW); // Matikan LED 1
//       digitalWrite(ledPin2, LOW); // Matikan LED 2
//       Serial.println("Pompa air menyala");
//       lcd.setCursor(0, 1);
//       lcd.print("Pompa: ON ");
//     }

//     // Data tidak valid
//     else {
//       Serial.println("Perintah tidak valid");
//       lcd.setCursor(0, 1);
//       lcd.print("Invalid Command");
//     }
//   }

//   // Kontrol pompa air dan LED berdasarkan nilai sensor dan ambang batas
//   else {
//     if (moistureValue < moistureThreshold && !pumpOn) {
//       digitalWrite(relayPin, HIGH);
//       pumpOn = true; // Set status pompa air menjadi menyala
//       digitalWrite(ledPin1, HIGH); // Nyalakan LED 1
//       digitalWrite(ledPin2, HIGH); // Nyalakan LED 2
//       Serial.println("Pompa air mati (sensor)");
//       lcd.setCursor(0, 1);
//       lcd.print("Pompa: OFF  ");
//     } else if (moistureValue >= moistureThreshold && pumpOn) {
//       digitalWrite(relayPin, LOW);
//       pumpOn = false; // Set status pompa air menjadi mati
//       digitalWrite(ledPin1, LOW); // Matikan LED 1
//       digitalWrite(ledPin2, LOW); // Matikan LED 2
//       Serial.println("Pompa air menyala (sensor)");
//       lcd.setCursor(0, 1);
//       lcd.print("Pompa: ON ");
//     }
//   }

//   delay(1000); // Tunggu 1 detik
// }
