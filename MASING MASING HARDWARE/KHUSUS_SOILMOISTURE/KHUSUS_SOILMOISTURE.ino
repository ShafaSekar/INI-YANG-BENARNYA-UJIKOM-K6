#include <LiquidCrystal_I2C.h>

#define RELAY_PUMP //masukkan pin relay pompa

LiquidCrystal_I2C lcd(0x27, 20, 4);

const int soilPin = ; //masukkan pin sensor soil moisture
int soilValue;
int soilPercentage;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("Selamat Datang!");
  lcd.setCursor(0, 1);
  lcd.print("WS Agroteknologi IoT");
  lcd.setCursor(3, 3);
  lcd.print("-- UG MURO --");
  delay(5000);
  lcd.clear();
  delay(2000);

  pinMode(soilPin, INPUT);

  pinMode(RELAY_PUMP, OUTPUT);
  // Initialize relay to OFF state
  digitalWrite(RELAY_PUMP, HIGH);  // HIGH to turn relay OFF
}

void loop() {
  soilValue = analogRead(soilPin);                   // Membaca nilai analog dari sensor
  soilPercentage = map(soilValue, 2048, 0, 0, 100);  // pakai mapping 2048 untuk 3v, 4095 untuk 5v
  soilPercentage = constrain(soilPercentage, 0, 100);

  // Display data on LCD
  lcd.setCursor(5, 0);
  lcd.print("Monitoring");

  lcd.setCursor(0, 1);
  lcd.print("K.Tanah: ");
  lcd.setCursor(9, 1);
  lcd.print("       "); //6 spasi
  lcd.setCursor(9, 1);
  lcd.print(soilPercentage);
  lcd.setCursor(17, 1);
  lcd.print("%");

  // Algoritma untuk menyalakan pompa
  if (soilPercentage < 30) {
    digitalWrite(RELAY_PUMP, HIGH); // Pompa menyala
  } else {
    digitalWrite(RELAY_PUMP, LOW); // Pompa mati
  }

  delay(1000);
}
