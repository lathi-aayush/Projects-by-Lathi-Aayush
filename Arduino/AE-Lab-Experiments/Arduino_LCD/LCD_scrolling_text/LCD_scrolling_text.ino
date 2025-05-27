#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCurso(5, 0);
  lcd.print("Scrolling");  
  delay(1500);      
}

void loop() {
  lcd.scrollDisplayLeft();  
  delay(300);               
}
