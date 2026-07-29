#include <WiFi.h>
#include <ThingSpeak.h>
#include <LiquidCrystal_I2C.h>
#include <elapsedMillis.h>

#define RELAY_PUMP //isi pin relay pompa

const char* ssid = "UGMURO-INET";              // Ganti dengan SSID WiFi kamu
const char* password = "Gepuk15000";           // Ganti dengan password WiFi kamu
const char* writeAPIKey = "GD6UJDLXW1LJJFIS";  // Ganti dengan Write API Key ThingSpeak kamu
const unsigned long channelID = 2604635;       // Ganti dengan Channel ID ThingSpeak kamu

//INTERVAL MILLIS
unsigned long thingSpeakInterval = 15000;
unsigned long sensorInterval = 500;
unsigned long displayInterval = 1000;

WiFiClient client;

elapsedMillis thingSpeakMillis;
elapsedMillis sensorMillis;
elapsedMillis displayMillis;
LiquidCrystal_I2C lcd(0x27, 20, 4);

const int soilPin = 34; //
int soilValue;
int soilPercentage;

void setup() {
  Serial.begin(115200);

  lcd.begin();
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
  digitalWrite(RELAY_PUMP, HIGH); // HIGH = relay OFF

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  ThingSpeak.begin(client);

  lcd.setCursor(5, 0);
  lcd.print("Monitoring");
  lcd.setCursor(0, 1);
  lcd.print("K.Tanah: ");
}

void loop() {
  if (sensorMillis >= sensorInterval) {
    //dijalankan per 0,5 detik sekali
    soilValue = analogRead(soilPin);                   // Membaca nilai analog dari sensor
    soilPercentage = map(soilValue, 4095, 0, 0, 100);  // Membalik rentang konversi
    soilPercentage = constrain(soilPercentage, 0, 100);

    if (soilPercentage < 30) {
      digitalWrite(RELAY_PUMP, HIGH); // Pompa menyala
    } else {
      digitalWrite(RELAY_PUMP, LOW); // Pompa mati
    }

    Serial.print("Soil: ");
    Serial.print(soilPercentage);
    Serial.println("%");
    sensorMillis = 0;
  }

  if (displayMillis >= displayInterval) {
    // Display data on LCD
    lcd.setCursor(5, 0);
    lcd.print("Monitoring");

    lcd.setCursor(9, 1);
    lcd.print("       ");  //6 spasi
    lcd.setCursor(9, 1);
    lcd.print(soilPercentage);
    lcd.setCursor(17, 1);
    lcd.print("%");

    displayMillis = 0;
  }

  if (thingSpeakMillis >= thingSpeakInterval) {
    // Kirim data ke ThingSpeak
    ThingSpeak.setField(1, soilPercentage);

    int x = ThingSpeak.writeFields(channelID, writeAPIKey);
    if (x == 200) {
      Serial.println("Update successful.");
    } else {
      Serial.println("Update failed. HTTP error code: " + String(x));
    }
    thingSpeakMillis = 0;
  }
}
