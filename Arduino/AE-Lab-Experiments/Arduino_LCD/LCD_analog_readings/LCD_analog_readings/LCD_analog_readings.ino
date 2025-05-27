#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  lcd.init();             
  lcd.backlight();        
}

void loop() {
  int sensorValue = analogRead(A0);               
  float voltage = sensorValue * (5.0 / 1023.0);    

  lcd.clear();                 
  lcd.setCursor(0, 0);
  lcd.print("Raw: ");
  lcd.print(sensorValue);      

  lcd.setCursor(0, 1);
  lcd.print("Volt: ");
  lcd.print(voltage, 2);       

  delay(1000);                 
}
