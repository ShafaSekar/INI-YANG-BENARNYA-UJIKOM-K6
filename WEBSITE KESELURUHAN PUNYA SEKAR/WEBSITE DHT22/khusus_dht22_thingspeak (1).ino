/* =========================================================
   KHUSUS DHT22 + KIPAS + THINGSPEAK
   (tanpa soil moisture, tanpa pompa)
   Isi bagian yang ada tanda ISI DI SINI sebelum upload ya!
   ========================================================= */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// ---------- ISI DI SINI: pin ----------
#define DHTPIN 2          // pin data DHT22 kamu
#define RELAY_FAN 5       // pin relay kipas kamu
#define DHTTYPE DHT22

// ---------- ISI DI SINI: WiFi & ThingSpeak ----------
const char* ssid       = "ISI_NAMA_WIFI";
const char* password   = "ISI_PASSWORD_WIFI";
unsigned long channelID = 0;              // ISI_CHANNEL_ID
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
  digitalWrite(RELAY_FAN, LOW);

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
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Gagal baca DHT22!");
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!  ");
    delay(2000);
    return;
  }

  // Kontrol kipas: nyala kalau panas & lembap
  bool fanOn = (temperature > 24 && humidity > 70);
  digitalWrite(RELAY_FAN, fanOn ? HIGH : LOW);

  // Tampilan LCD
  lcd.setCursor(0, 0);
  lcd.print("Suhu   : ");
  lcd.print(temperature, 1);
  lcd.print((char)223);
  lcd.print("C  ");

  lcd.setCursor(0, 1);
  lcd.print("K.Udara: ");
  lcd.print(humidity, 1);
  lcd.print("%  ");

  Serial.print("Suhu: "); Serial.print(temperature);
  Serial.print(" C, Kelembapan: "); Serial.print(humidity);
  Serial.print(" %, Kipas: "); Serial.println(fanOn ? "ON" : "OFF");

  // Kirim ke ThingSpeak tiap sendInterval
  if (millis() - lastSend > sendInterval) {
    ThingSpeak.setField(1, temperature); // field1 = suhu
    ThingSpeak.setField(2, humidity);    // field2 = kelembapan udara
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
