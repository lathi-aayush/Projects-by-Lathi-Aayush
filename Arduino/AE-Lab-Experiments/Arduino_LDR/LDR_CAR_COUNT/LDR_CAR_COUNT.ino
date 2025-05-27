#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int ldrPin = A0;
int ledPin = 8;
int threshold = 300;
int counter = 0;

bool objectPreviouslyDetected = false;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();

  pinMode(ledPin, OUTPUT);
}

void loop() {
  int ldrValue = analogRead(ldrPin);

  // Show current LDR value
  lcd.setCursor(0, 1);
  lcd.print("LDR: ");
  lcd.print(ldrValue);
  lcd.print("   "); // Clear any leftover digits

  if (ldrValue < threshold && !objectPreviouslyDetected) {
    counter++;
    objectPreviouslyDetected = true;

    digitalWrite(ledPin, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Car Count: ");
    lcd.print(counter);
    lcd.print("   "); // Clear any leftover digits
  } else if (ldrValue >= threshold) {
    objectPreviouslyDetected = false;

    digitalWrite(ledPin, LOW);
    lcd.setCursor(0, 0);
    lcd.print("No Vehicle     "); // Clear leftover chars
  }

  delay(200); // Smooth response
}
