/* =========================================================
   KHUSUS SOIL MOISTURE + POMPA + THINGSPEAK
   (tanpa DHT22)
   Isi bagian yang ada tanda ISI DI SINI sebelum upload ya!
   ========================================================= */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// ---------- ISI DI SINI: pin ----------
const int soilPin = A0;      // pin analog sensor soil moisture kamu
#define RELAY_PUMP 5         // pin relay pompa kamu

// Kalibrasi sensor (sesuaikan dengan sensor kamu)
const int soilDry = 1023;    // nilai analog saat tanah kering
const int soilWet = 300;     // nilai analog saat tanah basah

// ---------- ISI DI SINI: WiFi & ThingSpeak ----------
const char* ssid       = "ISI_NAMA_WIFI";
const char* password   = "ISI_PASSWORD_WIFI";
unsigned long channelID = 0;              // ISI_CHANNEL_ID
const char* writeAPIKey = "ISI_WRITE_API_KEY";

LiquidCrystal_I2C lcd(0x27, 16, 2);
WiFiClient client;

unsigned long lastSend = 0;
const unsigned long sendInterval = 15000; // kirim tiap 15 detik

void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();

  pinMode(RELAY_PUMP, OUTPUT);
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
  int soilRaw = analogRead(soilPin);
  int soilPercentage = map(soilRaw, soilDry, soilWet, 0, 100);
  soilPercentage = constrain(soilPercentage, 0, 100);

  // Kontrol pompa: nyala kalau tanah kering
  bool pumpOn = (soilPercentage < 30);
  digitalWrite(RELAY_PUMP, pumpOn ? HIGH : LOW);

  // Tampilan LCD
  lcd.setCursor(0, 0);
  lcd.print("K.Tanah: ");
  lcd.print(soilPercentage);
  lcd.print("%   ");

  lcd.setCursor(0, 1);
  lcd.print("Pompa  : ");
  lcd.print(pumpOn ? "ON " : "OFF");

  Serial.print("Kelembapan Tanah: "); Serial.print(soilPercentage);
  Serial.print(" %, Pompa: "); Serial.println(pumpOn ? "ON" : "OFF");

  // Kirim ke ThingSpeak tiap sendInterval
  if (millis() - lastSend > sendInterval) {
    ThingSpeak.setField(1, soilPercentage); // field1 = kelembapan tanah
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
