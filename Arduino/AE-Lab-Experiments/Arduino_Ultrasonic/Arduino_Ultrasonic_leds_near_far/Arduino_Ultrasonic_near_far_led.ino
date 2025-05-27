#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int trigPin = 9;
int echoPin = 10;

int redled = 2;     //near
int yellowled = 3;  //med
int greenled = 4;   //far

int farfarValue = 150;
int farValue = 60;

void setup() {

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initialising Radar");

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(redled, OUTPUT);
  pinMode(yellowled, OUTPUT);
  pinMode(greenled, OUTPUT);

  delay(1000);
}

void loop() {
  long duration;
  float distance;

  digitalWrite(trigPin, LOW);
  delay(2);
  
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = (duration * 0.0343) / 2;

  lcd.setCursor(0, 0);
  lcd.print("Distance : ");
  lcd.print(distance, 3);

  digitalWrite(redled, LOW);
  digitalWrite(yellowled, LOW);
  digitalWrite(greenled, LOW);

  lcd.setCursor(0, 1);
  if (distance > farfarValue) {
    lcd.print("Far        ");
    digitalWrite(greenled, HIGH);
  } else if (distance > farValue) {
    lcd.print("Mid-Far      ");
    digitalWrite(yellowled, HIGH);
  } else {
    lcd.print("Near       ");
    digitalWrite(redled, HIGH);
  }

  delay(1000);
}
