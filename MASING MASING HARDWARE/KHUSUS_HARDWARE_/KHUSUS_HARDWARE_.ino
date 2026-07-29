#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN //masukkan pin sensor DHT
#define DHTTYPE DHT21
#define RELAY1_PIN //masukkan pin relay 1 
#define RELAY2_PIN //masukkan pin relay 2
#define RELAY3_PIN //masukkan pin relay 3

DHT dht(DHTPIN, DHTTYPE);
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
  dht.begin();

  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  // Initialize relays to OFF state
  digitalWrite(RELAY1_PIN, HIGH);  // HIGH to turn relay OFF
  digitalWrite(RELAY2_PIN, HIGH);  // HIGH to turn relay OFF
  digitalWrite(RELAY3_PIN, HIGH);  // HIGH to turn relay OFF
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int soilValue = analogRead(soilPin);                   // Membaca nilai analog dari sensor
  int soilPercentage = map(soilValue, 2048, 0, 0, 100);  // pakai mapping 2048 untuk 3v, 4095 untuk 5v
  soilPercentage = constrain(soilPercentage, 0, 100);

  // Display data on LCD
  lcd.setCursor(5, 0);
  lcd.print("Monitoring");

  lcd.setCursor(0, 1);
  lcd.print("Suhu   : ");
  lcd.setCursor(8, 1);
  lcd.print(temperature);
  lcd.setCursor(17, 1);
  lcd.print("C");

  lcd.setCursor(0, 2);
  lcd.print("K.Udara: ");
  lcd.setCursor(8, 2);
  lcd.print( humidity);
  lcd.setCursor(17, 2);
  lcd.print("%");

  lcd.setCursor(0, 3);
  lcd.print("K.Tanah: ");
  lcd.setCursor(9, 3);
  lcd.print("       "); //6 spasi
  lcd.setCursor(9, 3);
  lcd.print(soilPercentage);
  lcd.setCursor(17, 3);
  lcd.print("%");

  // Algoritma untuk menyalakan relay
  if (temperature > 24 && humidity > 70) {
    digitalWrite(RELAY1_PIN, HIGH); // Relay 1 menyala
  } else {
    digitalWrite(RELAY1_PIN, LOW); // Relay 1 mati
  }

  if (soilPercentage < 30) {
    digitalWrite(RELAY3_PIN, HIGH); // Relay 2 (pompa) menyala
  } else {
    digitalWrite(RELAY3_PIN, LOW); // Relay 2 (pompa) mati
  }

  delay(1000);
}