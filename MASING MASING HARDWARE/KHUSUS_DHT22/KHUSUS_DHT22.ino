#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN //masukkan pin sensor DHT
#define DHTTYPE DHT22
#define RELAY_PIN //masukkan pin relay (kipas/pendingin)

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4);

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

  dht.begin();

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // HIGH to turn relay OFF
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

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
  lcd.print(humidity);
  lcd.setCursor(17, 2);
  lcd.print("%");

  // Algoritma untuk menyalakan relay
  if (temperature > 24 && humidity > 70) {
    digitalWrite(RELAY_PIN, HIGH); // Relay menyala
  } else {
    digitalWrite(RELAY_PIN, LOW); // Relay mati
  }

  delay(1000);
}
