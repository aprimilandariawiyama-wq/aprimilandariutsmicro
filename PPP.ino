#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// Inisialisasi: Tentukan pin LCD dan Servo
// Alamat I2C: Coba 0x27 dulu. Kalau layar mati, ganti ke 0x3F.
LiquidCrystal_I2C lcd(0x27, 16, 2); 
Servo myServo;

// 2. DEFINISI PIN RANGKAIAN (Koneksi Anda)
const int LEDHIJAU     = 7;      // D7: LED Hijau (SIAP)
const int LEDBIRU     = 6;      // D6: LED Biru (PANGGILAN AKTIF)
const int BUTTONNEXT   = 10;     // D10: Tombol PANGGIL SELANJUTNYA (+1)
const int BUTTONRESET  = 8;      // D8: Tombol PANGGIL ULANG/SET ANTRIAN
const int servoPin     = 9;      // D9: Pin Sinyal Motor Servo (Indikator)
const int potensiometerPin = A0;    // A0: Pin Potensiometer (PILIH TARGET)

// 3. VARIABEL STATUS
int antrianSaatIni = 0;       // Nomor antrian yang sedang dipanggil
int antrianTarget = 0;        // Nomor antrian yang dipilih oleh Potensiometer
const int antrianMaks = 50;   // Maksimal Nomor Antrian
int servoPosisiSiap = 0;      // Posisi Servo saat SIAP/STANDBY
int servoPosisiPanggil = 60;  // Posisi Servo saat dipanggil
const int durasiPanggil = 1500; // Durasi LED Hijau menyala (1.5 detik)

void setup() {
  
  // A. Pin Mode: Input atau Output
  pinMode(LEDHIJAU, OUTPUT);
  pinMode(LEDBIRU, OUTPUT);
  // INPUT_PULLUP: Tombol terhubung ke GND. Saat ditekan, terbaca LOW.
  pinMode(BUTTONNEXT, INPUT_PULLUP);
  pinMode(BUTTONRESET, INPUT_PULLUP);

  // B. Inisialisasi LCD dan Servo
  lcd.init();
  lcd.backlight();
  myServo.attach(servoPin);
  myServo.write(servoPosisiSiap); // Servo ke posisi STANDBY (0 derajat)
  
  // C. Status Awal
  digitalWrite(LEDBIRU, LOW);
  digitalWrite(LEDHIJAU, HIGH); // LED Hijau menyala: SISTEM SIAP
  
  lcd.setCursor(0, 0);
  lcd.print("SISTEM ANTRIAN");
  lcd.setCursor(0, 1);
  lcd.print("Antrian: 00");
  
  delay(2000);
  lcd.clear();

}

void loop() {
  // put your main code here, to run repeatedly:
  // 1. BACA INPUT POTENSIOMETER (Untuk memilih nomor antrian TARGET)
  int potensiometerValue = analogRead(potensiometerPin);
  antrianTarget = map(potensiometerValue, 0, 1023, 0, antrianMaks);

  // Serial.println(potensiometerValue);
   // 2. KONTROL TOMBOL PANGGIL SELANJUTNYA (+1)
  // int b = digitalRead(BUTTONNEXT);
  // Serial.println(b);
  if (digitalRead(BUTTONNEXT) == HIGH) { //KIE MIKI LOW
    // Tombol D10 ditekan
    delay(200); // Tunggu sebentar (Debounce) agar tidak naik terlalu cepat

    // Pastikan tidak melebihi batas maksimal antrian
    if (antrianSaatIni < antrianMaks) {
        antrianSaatIni++; // Nomor antrian naik 1
        digitalWrite(LEDHIJAU, HIGH);
        myServo.write(servoPosisiPanggil);
        delay(durasiPanggil);
    }
  }else{
      digitalWrite(LEDHIJAU, LOW);
      myServo.write(servoPosisiSiap);
    }
// 3. KONTROL TOMBOL PANGGIL ULANG/SET (RESET)
  if (digitalRead(BUTTONRESET) == HIGH) { 
    // Tombol D8 ditekan
    delay(200); // Debounce
    digitalWrite(LEDBIRU, HIGH);
    myServo.write(servoPosisiSiap);
    antrianSaatIni = 0;
    delay(durasiPanggil);
    
    // Logika Ganda:
    // Jika Antrian Saat Ini = 0, atur ke Antrian TARGET Potensiometer.
    // Jika Antrian Saat Ini > 0, panggil ulang nomor saat ini.
  }else{
    digitalWrite(LEDBIRU, LOW);
  }



  // 4. TAMPILAN LCD
  
  // Baris 1: Nomor Panggilan
  lcd.setCursor(0, 0);
  lcd.print("PANGGILAN: ");
  // Tampilkan antrian dengan 2 digit (misal: 05, 12)
  lcd.print(antrianSaatIni); 
  
  // Baris 2: Nomor Target dan Status
  lcd.setCursor(0, 1);
  lcd.print("Target: ");
  lcd.print(antrianTarget); 
  lcd.print("  "); 
  







}
