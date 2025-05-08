#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// DHT22 settings
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// LCD settings: 20x4 display at address 0x27
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Motor control pins
const int in1 = 8;
const int in2 = 9;
const int enA = 10;

void setup() {
  Serial.begin(9600);

  // Initialize DHT
  dht.begin();

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Starting system...");

  // Initialize motor pins
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enA, OUTPUT);

  // Set motor direction
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  delay(2000);
  lcd.clear();
}

void loop() {
  float temp = dht.readTemperature();
  String fanSpeed;

  if (isnan(temp)) {
    Serial.println("I need some FIX Bro!");
    lcd.setCursor(0, 0);
    lcd.print("Read Serial Monitor");
    delay(1000);
    return;
  }

  // Determine fan speed
  if (temp < 29) {
    analogWrite(enA, 0);
    fanSpeed = "OFF";
  } else if (temp < 30) {
    analogWrite(enA, 85);
    fanSpeed = "LOW";
  } else if (temp < 31) {
    analogWrite(enA, 115);
    fanSpeed = "MEDIUM";
  } else {
    analogWrite(enA, 255);
    fanSpeed = "HIGH";
  }

  // Print to Serial
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" C | Fan: ");
  Serial.println(fanSpeed);

  // Print to LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp, 1);  // 1 decimal place
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Fan: ");
  lcd.print(fanSpeed);

  delay(1000);  // Update every 2 seconds
}
