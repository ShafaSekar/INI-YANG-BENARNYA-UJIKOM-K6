/* =========================================================
   GABUNGAN: DHT22 + SOIL MOISTURE + KIPAS + POMPA + THINGSPEAK
   Isi bagian yang ada tanda ISI DI SINI sebelum upload ya!
   ========================================================= */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// ---------- ISI DI SINI: pin DHT22 & kipas ----------
#define DHTPIN 2          // pin data DHT22 kamu
#define RELAY_FAN 5       // pin relay kipas kamu
#define DHTTYPE DHT22

// ---------- ISI DI SINI: pin soil moisture & pompa ----------
const int soilPin = A0;      // pin analog sensor soil moisture kamu
#define RELAY_PUMP 4          // pin relay pompa kamu

// Kalibrasi sensor soil (sesuaikan dengan sensor kamu)
const int soilDry = 1023;
const int soilWet = 300;

// ---------- ISI DI SINI: WiFi & ThingSpeak ----------
const char* ssid       = "ISI_NAMA_WIFI";
const char* password   = "ISI_PASSWORD_WIFI";
unsigned long channelID = 0;              // ISI_CHANNEL_ID (satu channel, 3 field)
const char* writeAPIKey = "ISI_WRITE_API_KEY";

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

unsigned long lastSend = 0;
const unsigned long sendInterval = 15000; // kirim tiap 15 detik

void setup() {
  Serial.begin(115200);
  dht.begin();

  lcd.init();
  lcd.backlight();

  pinMode(RELAY_FAN, OUTPUT);
  pinMode(RELAY_PUMP, OUTPUT);
  digitalWrite(RELAY_FAN, LOW);
  digitalWrite(RELAY_PUMP, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(400);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  lcd.clear();

  ThingSpeak.begin(client);
}

void loop() {
  // --- baca DHT22 ---
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Gagal baca DHT22!");
    lcd.setCursor(0, 0);
    lcd.print("Sensor DHT Error");
    delay(2000);
    return;
  }

  // --- baca soil moisture ---
  int soilRaw = analogRead(soilPin);
  int soilPercentage = map(soilRaw, soilDry, soilWet, 0, 100);
  soilPercentage = constrain(soilPercentage, 0, 100);

  // --- kontrol output ---
  bool fanOn  = (temperature > 24 && humidity > 70);
  bool pumpOn = (soilPercentage < 30);
  digitalWrite(RELAY_FAN, fanOn ? HIGH : LOW);
  digitalWrite(RELAY_PUMP, pumpOn ? HIGH : LOW);

  // --- tampilan LCD (bergantian tiap loop biar muat di 16x2) ---
  lcd.setCursor(0, 0);
  lcd.print("S:");
  lcd.print(temperature, 1);
  lcd.print((char)223);
  lcd.print("C H:");
  lcd.print(humidity, 0);
  lcd.print("%  ");

  lcd.setCursor(0, 1);
  lcd.print("Tanah:");
  lcd.print(soilPercentage);
  lcd.print("% ");
  lcd.print(fanOn ? "F" : "-");
  lcd.print(pumpOn ? "P" : "-");
  lcd.print(" ");

  Serial.print("Suhu:"); Serial.print(temperature);
  Serial.print(" Humid:"); Serial.print(humidity);
  Serial.print(" Soil:"); Serial.print(soilPercentage);
  Serial.print(" Fan:"); Serial.print(fanOn ? "ON" : "OFF");
  Serial.print(" Pump:"); Serial.println(pumpOn ? "ON" : "OFF");

  // --- kirim ke ThingSpeak tiap sendInterval ---
  if (millis() - lastSend > sendInterval) {
    ThingSpeak.setField(1, temperature);     // field1 = suhu
    ThingSpeak.setField(2, humidity);        // field2 = kelembapan udara
    ThingSpeak.setField(3, soilPercentage);  // field3 = kelembapan tanah
    int result = ThingSpeak.writeFields(channelID, writeAPIKey);
    if (result == 200) {
      Serial.println("Data terkirim ke ThingSpeak!");
    } else {
      Serial.println("Gagal kirim, kode error: " + String(result));
    }
    lastSend = millis();
  }

  delay(2000);
}
