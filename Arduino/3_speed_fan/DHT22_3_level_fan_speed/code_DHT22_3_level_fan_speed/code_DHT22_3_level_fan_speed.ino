#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// LCD: 20x4 display at I2C address 0x27
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Fan control pins
const int in1 = 8;
const int in2 = 9;
const int enA = 10;

bool fanEnabled = true;  // Manual switch via Serial

void setup() {
  Serial.begin(9600);
  dht.begin();

  // Motor pin setup
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enA, OUTPUT);

  // Set fan direction
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  // LCD init
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Fan System Ready");

  Serial.println("Type 'fanon' or 'fanoff' to control the fan manually.");
}

void loop() {
  // Check Serial for manual ON/OFF
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.equalsIgnoreCase("fanon")) {
      fanEnabled = true;
      Serial.println("Fan control: ON");
    } else if (command.equalsIgnoreCase("fanoff")) {
      fanEnabled = false;
      Serial.println("Fan control: OFF");
    }
  }

  float temp = dht.readTemperature();
  String fanStatus = "OFF";

  if (isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error");
    delay(1000);
    return;
  }

  if (fanEnabled) {
    if (temp < 30.7) {
      analogWrite(enA, 0);
      fanStatus = "OFF";
    } else if (temp < 31) {
      analogWrite(enA, 255);  // Start with full power
      delay(200);             // Run for 200ms
      analogWrite(enA, 85);   // Then reduce to low speed
      fanStatus = "LOW";
    } else if (temp < 31.5) {
      analogWrite(enA, 255);
      delay(200);
      analogWrite(enA, 115);
      fanStatus = "MEDIUM";
    } else {
      analogWrite(enA, 255);  // Already full speed
      fanStatus = "HIGH";
    }

  } else {
    analogWrite(enA, 0);  // Force off
    fanStatus = "MANUAL OFF";
  }

  // Print to Serial
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" °C | Fan: ");
  Serial.println(fanStatus);

  // Print to LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp, 1);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Fan: ");
  lcd.print(fanStatus);

  delay(1000);
}
