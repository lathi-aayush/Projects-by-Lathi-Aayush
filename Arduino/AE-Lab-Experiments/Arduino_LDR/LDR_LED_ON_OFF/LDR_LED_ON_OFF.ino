#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int ldrPin = A0;      // LDR connected to analog pin A0
int ledPin = 8;       // LED connected to digital pin 8
int threshold = 400;  // Adjust based on your lighting conditions

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.print("Initialising System...");
  pinMode(ledPin, OUTPUT);
}

void loop() {
  int ldrValue = analogRead(ldrPin);  // Read LDR value
  Serial.println(ldrValue);           // Print value for calibration
  lcd.clear();
  lcd.setCursor(0, 0);

  if (ldrValue < threshold) {
    digitalWrite(ledPin, HIGH);
    lcd.print("Too Dark:ON");  // It's dark - turn on LED
  } else {
    digitalWrite(ledPin, LOW);      // It's light - turn off LED
    lcd.print("Enough Light:OFF");  // It's dark - turn on LED
  }
  lcd.setCursor(0, 1);
  lcd.print("LDR : ");
  lcd.println(ldrValue);
  delay(1000);
}
