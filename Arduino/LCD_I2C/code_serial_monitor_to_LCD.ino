#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);  // 20x4 LCD with I2C address 0x27

String incomingText = "";

void setup()
{
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Waiting for input");

  Serial.begin(9600);  // Start serial communication
}

void loop()
{
  // Check for input terminated by Enter key (newline character)
  if (Serial.available()) {
    incomingText = Serial.readStringUntil('\n');  // Read input until Enter

    lcd.clear();
    lcd.setCursor(0, 0);
   
    if (incomingText.length() > 0) {
      lcd.print(incomingText.substring(0, 20));  // Show first 20 chars
    } else {
      lcd.print("Empty input");
    }
  }
}


