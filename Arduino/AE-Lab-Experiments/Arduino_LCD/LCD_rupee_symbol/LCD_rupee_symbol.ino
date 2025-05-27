#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte rupee[8] = {
  B11111,  
  B00010,  
  B11111,  
  B00010,  
  B01100,  
  B11000,  
  B01100,  
  B00111   
};    


void setup() {
  lcd.init();
  lcd.backlight();

  lcd.createChar(0, rupee);  
  lcd.setCursor(0, 0);
  lcd.write(byte(0));  
  lcd.print(" 500");   
}
void loop() {
}